#ifndef _MSGSVR_SERVER_H
#define _MSGSVR_SERVER_H

extern int msgsvr_server_init(
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
  );

#endif