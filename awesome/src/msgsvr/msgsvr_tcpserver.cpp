#include "msgsvr_tcpserver.h"
#include "msgsvr_libevent_tcpserver.h"
#include "msgsvr_buffer.h"
#include "msgsvr_queue.h"


typedef struct {
  char remote_ip[MSGSVR_SIZE_IP];
  unsigned short remote_port;
  //msgsvr_buffer_t buff;
} msgsvr_tcpclient_t;

#define MsgsvrTCPClientMap  std::map<std::string, msgsvr_tcpclient_t *>

typedef struct {
  void *context;
  char local_ip[MSGSVR_SIZE_IP];
  unsigned short local_port;
  unsigned int buffer_size;
  msgsvr_tcpserver_received_callback received;
  msgsvr_tcpserver_connected_callback connected;
  msgsvr_tcpserver_closed_callback closed;
  void *args;
  MsgsvrTCPClientMap *clients;
  pthread_mutex_t mutex;
} msgsvr_tcpserver_t;

static int msgsvr_tcpserver_recv(
  void *handler,
  void *args,
  const char *local_ip,
  unsigned short local_port,
  const char *remote_ip,
  const char *remote_port,
  const char *buff,
  int buff_len) {

}

static int msgsvr_tcpserver_connected(
  void *handler,
  void *args,
  const char *local_ip,
  unsigned short local_port,
  const char *remote_ip,
  unsigned short remote_port) {

}

static int msgsvr_tcpserver_closed(
  void *handler,
  void *arg,
  const char *local_ip,
  unsigned short local_port,
  const char *remote_ip,
  unsigned short remote_port) {


}

int msgsvr_tcpserver_send(void *context, msgsvr_message_t *message) {

}

int msgsvr_tcpserver_close(
  void *context,
  const char *remote_ip,
  unsigned short remote_port) {

}

int msgsvr_tcpserver_create(
  void **handler,
  char *local_ip,
  unsigned short *local_port,
  unsigned int max_connectors,
  unsigned int buffer_size,
  int recv_buffer_size,
  int send_buffer_size,
  msgsvr_tcpserver_received_callback received,
  msgsvr_tcpserver_connected_callback connected,
  msgsvr_tcpserver_closed_callback closed,
  void *args
  ) {
  msgsvr_tcpserver_t *tcpserver = (msgsvr_tcpserver_t *)malloc(sizeof(msgsvr_tcpserver_t));
  tcpserver->mutex = PTHREAD_MUTEX_INITIALIZER;
  tcpserver->received = received;
  tcpserver->connected = connected;
  tcpserver->closed = closed;
  tcpserver->args = args;
  sprintf(tcpserver->local_ip, "%s", local_ip);
  tcpserver->local_port = *local_port;
  tcpserver->buffer_size = buffer_size;
  tcpserver->clients = new (MsgsvrTCPClientMap);

  if (msgsvr_libevent_tcpserver_create(
    &tcpserver->context,
    local_ip,
    local_port,
    max_connectors,
    send_buffer_size,
    recv_buffer_size,
    msgsvr_tcpserver_recv,
    msgsvr_tcpserver_connected,
    msgsvr_tcpserver_closed,
    tcpserver
    ) != OK) {
    LOG_ERROR("create libevent tcpserver[%s:%u] error.\n",
      local_ip, *local_port);
    return ERROR;
  }

  *handler = tcpserver;
  LOG_INFO("tcpserver[%s:%u] create successfully.\n");

  return OK;
}
