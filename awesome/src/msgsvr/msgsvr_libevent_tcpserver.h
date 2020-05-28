#ifndef _MSGSVR_LIBEVENT_TCPSERVER_H
#define _MSGSVR_LIBEVENT_TCPSERVER_H

#include "msgsvr_util.h"

typedef int (*msgsvr_libevent_tcpserver_recv_callback) (void *handler, void *args, const char *local_ip, unsigned short local_port, const char *remote_ip, const char *remote_port, const char *buff, int buff_len);
typedef int (*msgsvr_libevent_tcpserver_connected_callback) ( void *handler, void *args, const char *local_ip, unsigned short local_port, const char *remote_ip, unsigned short remote_port);
typedef int (*msgsvr_libevent_tcpserver_closed_callback) (void *handler, void *arg, const char *local_ip, unsigned short local_port, const char *remote_ip, unsigned short remote_port);


int msgsvr_libevent_tcpserver_create(
  void **handler,
  char *local_ip,
  unsigned short *local_port,
  unsigned int max_connectors,
  int recv_buffer_size,
  int send_buffer_size,
  msgsvr_libevent_tcpserver_recv_callback received,
  msgsvr_libevent_tcpserver_connected_callback connected,
  msgsvr_libevent_tcpserver_closed_callback closed,
  void *args
  );

#endif