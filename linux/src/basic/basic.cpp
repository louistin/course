#include "basic.h"
#include "callback/basic_sync_callback.h"
#include "callback/basic_async_callback.h"

int basic_init() {
    easylogger_init();
    elog_set_text_color_enabled(false);

    basic_sync_callback_init();
    basic_async_callback_init();

    LOG_INFO("basic init ok.");

    return 0;
}