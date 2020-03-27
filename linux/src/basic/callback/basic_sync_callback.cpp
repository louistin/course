#include "basic_sync_callback.h"
#include "basic_sync_callback_define.h"

void basic_sync_callback_fun(int num, const char *str) {
    LOG_INFO("basic_sync_callback_fun start.");
    LOG_INFO("num: %d, str: %s", num, str);
    sleep(1);
    LOG_INFO("basic_sync_callback_fun end.");
}

int basic_sync_callback_init() {
    basic_sync_set_cb(10, "hello", basic_sync_callback_fun);

    return 0;
}