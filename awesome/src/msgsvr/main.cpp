#include "msgsvr.h"
#include "msgsvr_server.h"

int main(int argc, char const *argv[])
{
  easylogger_init();
  elog_set_text_color_enabled(false);

  void *handler = NULL;
  char local_ip[MSGSVR_SIZE_IP];
  unsigned short local_port;

  memset(local_ip, 0, sizeof(local_ip));
  sprintf(local_ip, "%s", "127.0.0.1");
  local_port = 9999;

  /*
    void **handler,
    const char *transport,
    const char *type,
    char *local_ip,
    unsigned short *local_port,
    unsigned int max_connectors,
    unsigned int buffer_size,
    int queue_size,
    int send_buffer_size,
    int recv_buffer_size
  */
  if (msgsvr_server_init(
    &handler,
    "tcp",
    "json",
    local_ip,
    &local_port,
    1024,
    1024,
    32,
    1024,
    1024
    ) != OK) {
    LOG_ERROR("msgsvr server init error.\n");
    return ERROR;
  }

  LOG_INFO("msgsvr server init ok.\n");

  while (1) {
    sleep(1);
  }

  return OK;
}
