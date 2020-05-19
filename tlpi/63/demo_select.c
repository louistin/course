#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  fd_set readfds, writefds;
  int ready, nfds, fd, num_read, j;
  struct timeval timeout;
  struct timeval *pto;
  char buf[16];

  // 修改 pto 值为 NULL 或 timeout 值为 0, 大于 0
  pto = &timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  nfds = 0;
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);

  nfds = nfds > 0 ? nfds : 0 + 1;
  FD_SET(0, &readfds);

  nfds = nfds > 1 ? nfds : 1 + 1;
  FD_SET(1, &writefds);

  ready = select(nfds, &readfds, &writefds, NULL, pto);
  if (ready == -1) {
    perror("select");
    return -1;
  }

  printf("ready: %d\n", ready);

  for (fd = 0; fd < nfds; fd++) {
    printf("%d: %s%s\n", fd, FD_ISSET(fd, &readfds) ? "r" : "", FD_ISSET(fd, &writefds) ? "w" : "");
  }

  if (pto != NULL) {
    printf("timeout after select(): %ld.%03ld\n", (long) timeout.tv_sec, (long) timeout.tv_usec / 1000);
  }

  return 0;
}
