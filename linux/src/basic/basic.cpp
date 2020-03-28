#include "basic.h"

#ifdef __BASIC_CALLBACK
#include "callback/basic_sync_callback.h"
#include "callback/basic_async_callback.h"
#endif

#ifdef __BASIC_EPOLL_SERVER
#include "io/basic_epoll_server.h"
#endif

#ifdef __BASIC_EPOLL_CLIENT
#include "io/basic_epoll_client.h"
#endif

int basic_init() {
    easylogger_init();
    elog_set_text_color_enabled(false);

#ifdef __BASIC_CALLBACK
    basic_sync_callback_init();
    basic_async_callback_init();
#endif

#ifdef __BASIC_EPOLL_SERVER
    basic_epoll_server_init();
#endif

#ifdef __BASIC_EPOLL_CLIENT
    basic_epoll_client_init();
#endif

    LOG_INFO("basic init ok.");

    return 0;
}