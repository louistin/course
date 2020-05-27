#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "elog.h"
#include "elog_cfg.h"
#include "event.h"
#include "event-config.h"

int main(int argc, char const *argv[])
{
  easylogger_init();
  elog_set_text_color_enabled(false);

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd <= 0) {
    LOG_ERROR("create socket error.\n");
    return -1;
  }

  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
  saddr.sin_port = htons(10001);
  bind(fd, (struct sockaddr *)&saddr, sizeof(saddr));

  struct event_base *base;
  struct event eventfd;

  base = event_init();
  if (base == NULL) {
    LOG_ERROR("event init error.\n");
    return -1;
  }

  return 0;
}
