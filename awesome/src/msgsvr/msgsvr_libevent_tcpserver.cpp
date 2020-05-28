#include "msgsvr_libevent_tcpserver.h"

static void msgsvr_libevent_read(struct bufferevent *bev, void *ctx) {
  struct evbuffer *input = bufferevent_get_input(bev);
  //struct evbuffer *output = bufferevent_get_output(bev);

  LOG_INFO("msgsvr libevent read callback.\n");

  // 这里是不是需要调用 msgsvr_libevent_tcpserver_recv_callback received
}

static void msgsvr_libevent_event(
  struct bufferevent *bev,
  short events,
  void *ctx) {
  LOG_INFO("msgsvr libevent event callback.\n");

  // 这里是不是需要调用 msgsvr_libevent_tcpserver_closed_callback closed
}

static void msgsvr_libevent_accept_connect(
  struct evconnlistener *listener,
  evutil_socket_t fd,
  struct sockaddr *address,
  int socklen,
  void *ctx) {
  struct event_base *base = evconnlistener_get_base(listener);
  struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(bev, msgsvr_libevent_read, NULL, msgsvr_libevent_event, NULL);
  bufferevent_enable(bev, EV_READ | EV_WRITE);

  LOG_INFO("msgsvr libevent accept connect callback.\n");

  // 这里是不是需要调用 msgsvr_libevent_tcpserver_connected_callback connected
}

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
  ) {
  struct event_base *base;
  struct evconnlistener *listener;
  struct sockaddr_in sin;

  base = event_base_new();
  if (base == NULL) {
    LOG_ERROR("event base new error.\n");
    return ERROR;
  }

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(0);
  if (local_port != NULL && *local_port > 0) {
    sin.sin_port = *local_port;
  } else {
    sin.sin_port = 0;
  }

  //LOG_INFO("sin.sin_port: %u\n", sin.sin_port);

  listener = evconnlistener_new_bind(
                base,
                msgsvr_libevent_accept_connect,
                NULL, // 需要传递回调函数
                LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
                -1,
                (struct sockaddr *)&sin,
                sizeof(sin));
  if (listener == NULL) {
    LOG_ERROR("create listener error. %s\n", strerror(errno));
    return ERROR;
  }

  LOG_INFO("create listener ok.\n");

  event_base_dispatch(base);

  return OK;
}