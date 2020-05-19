#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_BUF   1024
#define MAX_EVENTS  8

// $ mkfifo p q
// $ ./demo_epoll p q     # 终端 1
// $ cat > p              # 终端 2
int main(int argc, char const *argv[]) {
  int epfd, ready, fd, s, j, num_open_fds;
  struct epoll_event ev;
  struct epoll_event evlist[MAX_EVENTS];
  char buf[MAX_BUF];

  if (argc < 2 || strcasecmp(argv[1], "--help") == 0) {
    printf("usage: %s file ...\n", argv[0]);
    return -1;
  }

  epfd = epoll_create(argc - 1);
  if (epfd == -1) {
    printf("epoll_create error.\n");
    return -1;
  }

  for (j = 1; j < argc; j++) {
    fd = open(argv[j], O_RDONLY);
    if (fd == -1) {
      printf("open %s error.\n", argv[j]);
      return -1;
    }

    printf("opened \"%s\" on fd %d.\n", argv[j], fd);

    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
      printf("epoll_ctl error.\n");
      return -1;
    }

    num_open_fds = argc - 1;

    while (num_open_fds > 0) {
      printf("about to epoll_wait()\n");

      ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
      if (ready == -1) {
        if (errno == EINTR) {
          continue;
        } else {
          printf("epoll_wait error.\n");
          return -1;
        }
      }

      printf("ready: %d\n", ready);

      for (j = 0; j < ready; j++) {
        printf("  fd = %d; events: %s%s%s\n", evlist[j].data.fd,
                (evlist[j].events & EPOLLIN) ? "EPOLLIN " : "",
                (evlist[j].events & EPOLLHUP) ? "EPOLLHUP " : "",
                (evlist[j].events & EPOLLERR) ? "EPOLLERR " : "");

        if (evlist[j].events & EPOLLIN) {
          s = read(evlist[j].data.fd, buf, MAX_BUF);
          if (s == -1) {
            printf("read error.\n");
            return -1;
          }

          printf("    read %d bytes: %.*s\n", s, s, buf);

        } else if (evlist[j].events & (EPOLLHUP | EPOLLERR)) {
          printf("    closing fd %s\n", evlist[j].data.fd);
          if (close(evlist[j].data.fd) == -1) {
            printf("close error.\n");
            return -1;
          }

          num_open_fds--;
        }
      }
    }
  }

  printf("all file description closed; bye\n");

  return 0;
}