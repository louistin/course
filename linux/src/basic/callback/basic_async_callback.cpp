#include "basic_async_callback.h"
#include "basic_async_callback_define.h"

void basic_async_callback_fun(int num, const char *str) {
    LOG_INFO("basic_async_callback_fun start.");
    LOG_INFO("num: %d, str: %s", num, str);
    sleep(1);
    LOG_INFO("basic_async_callback_fun end.");
}

int basic_async_callback_init() {
    basic_async_set_cb(10, "hello", basic_async_callback_fun);

    return 0;
}