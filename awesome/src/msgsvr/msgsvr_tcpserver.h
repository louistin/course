#ifndef _MSGSVR_TCPSERVER_H
#define _MSGSVR_TCPSERVER_H

#include "msgsvr_util.h"

typedef int (*msgsvr_tcpserver_received_callback) (void *context, void *args, msgsvr_message_t *message);
typedef int (*msgsvr_tcpserver_connected_callback) (void *context, void *args, const char *remote_ip, unsigned short remote_port);
typedef int (*msgsvr_tcpserver_closed_callback) (void *context, void *args, const char *remote_ip, unsigned short remote_port);

extern int msgsvr_tcpserver_create(
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
  void *args);

extern int msgsvr_tcpserver_send(void *context, msgsvr_message_t *message);

extern int msgsvr_tcpserver_close(
  void *context,
  const char *remote_ip,
  unsigned short remote_port);

#endif