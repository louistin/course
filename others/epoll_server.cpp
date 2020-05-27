#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <netinet/in.h>

#define SERVER_PORT 10001
#define MAX_EVENTS  1024
#define BUFF_LEN    1024



typedef void (*event_callback)(int fd, int events, void *arg);

struct event_s {
  int fd;
  int events;
  void *arg;
  event_callback *call_back;
  int status;
  char buff[BUFF_LEN];
  int len;
  long last_active;
};

int g_epfd;
struct event_s g_events[MAX_EVENTS + 1];

void event_set(struct event_s *ev, int fd, event_callback call_back, void *arg) {
  ev->fd = fd;
  ev->call_back = call_back;
  ev->events = 0;
  ev->arg = arg;
  ev->status = 0;
  ev->last_active = time(NULL);
}

void accept_connect_cb(int fd, int events, void *arg) {
  struct sockaddr_in sa;
  socklen_t len = sizeof(sa);
  int cfd, i;

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

  } while (0);



}

void init_listen_socket(int epfd, unsigned short port) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd <= 0) {
    perror("socket");
    return;
  }
  fcntl(fd, F_SETFL, O_NONBLOCK);

  event_set(&g_events[MAX_EVENTS], fd, accept_connect_cb, &g_events[MAX_EVENTS]);


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



  return 0;
}
