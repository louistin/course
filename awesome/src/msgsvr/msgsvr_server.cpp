#include "msgsvr.h"
#include "msgsvr_server.h"
#include "msgsvr_tcpserver.h"
#include "msgsvr_queue.h"

typedef struct {
  void *context;
  void *client_context;
  char transport[MSGSVR_SIZE_TRANSPORT];
  char type[MSGSVR_SIZE_TYPE];
  char local_ip[MSGSVR_SIZE_IP];
  unsigned short local_port;
  void *recv_queue;
} msgsvr_server_t;

int msgsvr_server_init(
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
  ){
  *handler = NULL;
  msgsvr_server_t *server = (msgsvr_server_t *)malloc(sizeof(msgsvr_server_t));
  memset(server, 0, sizeof(msgsvr_server_t));
  sprintf(server->local_ip, "%s", local_ip);
  server->local_port = *local_port;
  sprintf(server->transport, "%s", transport);
  sprintf(server->type, "%s", type);

  if (msgsvr_queue_build(&server->recv_queue, queue_size) != OK) {
    LOG_ERROR("msgsvr queue build error.\n");
    return ERROR;
  }

  if (strcasecmp(transport, "tcp") == 0) {
    // 创建 tcpserver
    // 创建 tcpclient
  } else {
    LOG_ERROR("transport: %s is not support.\n", transport);
    return ERROR;
  }

  server->local_port = *local_port;
  *handler = server;

  LOG_INFO("msgsvr[%s:%u] create successfully.\n", local_ip, *local_port);

  return OK;
}