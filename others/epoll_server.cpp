#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define SERVER_PORT 10001
#define MAX_EVENTS 1024
#define BUFF_LEN 1024

typedef void (*event_callback)(int fd, int events, void *arg);

struct event_s {
  int fd;
  int events;
  void *arg;
  // MAJOR: EventHandler 事件处理接口
  // 一个或多个模板函数组成的接口,
  // 这些模板函数描述了和应用程序相关的对某个事件的操作
  event_callback call_back;
  int status;
  char buff[BUFF_LEN];
  int len;
  long last_active;
};

int g_epfd;
struct event_s g_events[MAX_EVENTS + 1];

// MAJOR: Reactor 管理器
// 定义了一些接口, 用于应用程序控制事件调度, 以及应用程序注册,
// 删除事件处理器和相关描述符. 也包括了 event_add, event_del
void event_set(struct event_s *ev, int fd, event_callback call_back,
               void *arg) {
  ev->fd = fd;
  ev->call_back = call_back;
  ev->events = 0;
  ev->arg = arg;
  ev->status = 0;
  ev->last_active = time(NULL);
}

void event_add(int epfd, int events, struct event_s *ev) {
  struct epoll_event epv;
  int op;

  epv.data.ptr = ev;
  epv.events = ev->events = events;

  if (ev->status == 1) {
    op = EPOLL_CTL_MOD;
  } else {
    op = EPOLL_CTL_ADD;
    ev->status = 1;
  }

  if (epoll_ctl(epfd, op, ev->fd, &epv) < 0) {
    printf("event add failed [fd = %d], events[%d]\n", ev->fd, events);
  } else {
    printf("event add ok [fd = %d], op = %d,  events[%X]\n", ev->fd, op,
           events);
  }
}

void event_del(int epfd, struct event_s *ev) {
  struct epoll_event epv;

  if (ev->status != 1) {
    return;
  }

  epv.data.ptr = ev;
  ev->status = 0;

  epoll_ctl(epfd, EPOLL_CTL_DEL, ev->fd, &epv);
}

// MAJOR: 具体的事件处理器
// 事件处理器接口的实现
void recv_cb(int fd, int events, void *arg);

void send_cb(int fd, int events, void *arg) {
  struct event_s *ev = (struct event_s *)arg;
  int len;

  len = send(fd, ev->buff, ev->len, 0);
  event_del(g_epfd, ev);
  if (len > 0) {
    printf("send[fd = %d] [%d] %s\n", fd, ev->len, ev->buff);

    event_set(ev, fd, recv_cb, ev);
    event_add(g_epfd, EPOLLIN, ev);
  } else {
    close(ev->fd);
    printf("send[fd = %s] error %s\n", fd, strerror(errno));
  }
}

void recv_cb(int fd, int events, void *arg) {
  struct event_s *ev = (struct event_s *)arg;
  int len;

  len = recv(fd, ev->buff, sizeof(ev->buff), 0);
  event_del(g_epfd, ev);

  if (len > 0) {
    ev->len = len;
    ev->buff[len] = '\0';
    printf("client[%d]: %s\n", fd, ev->buff);
    event_set(ev, fd, send_cb, ev);
    event_add(g_epfd, EPOLLOUT, ev);
  } else if (len == 0) {
    close(ev->fd);
    printf("[fd = %d] pos[%d] closed\n", fd, (int)(ev - g_events));
  } else {
    close(ev->fd);
    printf("recv[fd = %d] error[%d]: %s\n", fd, errno, strerror(errno));
  }
}

void accept_connect_cb(int fd, int events, void *arg) {
  struct sockaddr_in sa;
  socklen_t len = sizeof(sa);
  int cfd, i;

  // MAJOR: handle
  // 用于识别每一个事件
  cfd = accept(fd, (struct sockaddr *)&sa, &len);
  if (cfd == -1) {
    if (errno != EAGAIN && errno != EINTR) {
      // do nothing
    }

    printf("%s: accept, %s\n", __func__, strerror(errno));
    return;
  }

  do {
    // 从 g_events 中选择当前最小未使用的项
    for (i = 0; i < MAX_EVENTS; i++) {
      if (g_events[i].status == 0) {
        break;
      }
    }

    if (i == MAX_EVENTS) {
      printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
      break;
    }

    int flag = 0;
    flag = fcntl(cfd, F_SETFL, O_NONBLOCK);
    if (flag < 0) {
      printf("%s: fcntl nonblocking failed, %s\n", __func__, strerror(errno));
      break;
    }

    event_set(&g_events[i], cfd, recv_cb, &g_events[i]);
    event_add(g_epfd, EPOLLIN, &g_events[i]);
  } while (0);

  printf("new connect [%s:%d][time:%ld], pos[%d]\n", inet_ntoa(sa.sin_addr),
         ntohs(sa.sin_port), g_events[i].last_active, i);
}

void init_listen_socket(int epfd, unsigned short port) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd <= 0) {
    perror("socket");
    return;
  }

  fcntl(fd, F_SETFL, O_NONBLOCK);

  event_set(&g_events[MAX_EVENTS], fd, accept_connect_cb,
            &g_events[MAX_EVENTS]);
  event_add(epfd, EPOLLIN, &g_events[MAX_EVENTS]);

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);

  bind(fd, (struct sockaddr *)&sin, sizeof(sin));

  listen(fd, 20);
}

int main(int argc, char const *argv[]) {
  unsigned short port = SERVER_PORT;

  if (argc > 2) {
    port = atoi(argv[1]);
  }

  g_epfd = epoll_create(MAX_EVENTS + 1);
  if (g_epfd < 0) {
    printf("create epfd in %s error %s\n", __func__, strerror(errno));
    return -1;
  }

  init_listen_socket(g_epfd, port);

  struct epoll_event events[MAX_EVENTS + 1];
  printf("server running:port [%u]\n", port);

  int checkpos = 0;
  int i;

  while (1) {
    long now = time(NULL);
    for (i = 0; i < 100; i++, checkpos++) {
      if (checkpos == MAX_EVENTS) {
        checkpos = 0;
      }

      if (g_events[checkpos].status != 1) {
        continue;
      }

      long duration = now - g_events[checkpos].last_active;
      if (duration >= 100) {
        printf("fd[%d] timeout\n", g_events[checkpos].fd);
        close(g_events[checkpos].fd);
        event_del(g_epfd, &g_events[checkpos]);
      }
    }

    // MAJOR: demultiplexer 同步事件分离器
    // 等待一个或多个事件发生, 调用者阻塞直到分离器分离的描述符集上又事件发生
    int nfd = epoll_wait(g_epfd, events, MAX_EVENTS + 1, 1000);
    if (nfd < 0) {
      printf("epoll_wait error, exit.\n");
      break;
    }

    for (i = 0; i < nfd; i++) {
      struct event_s *ev = (struct event_s *)events[i].data.ptr;
      if ((events[i].events & EPOLLIN) && (ev->events && EPOLLIN)) {
        ev->call_back(ev->fd, events[i].events, ev->arg);
      }

      if ((events[i].events & EPOLLOUT) && (ev->events && EPOLLOUT)) {
        ev->call_back(ev->fd, events[i].events, ev->arg);
      }
    }
  }

  return 0;
}