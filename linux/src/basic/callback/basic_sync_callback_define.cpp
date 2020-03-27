#include "basic_sync_callback_define.h"

void basic_sync_set_cb(int num, const char *str, fun_cb callback) {
    LOG_INFO("basic_sync_set_cb start.");
    callback(num, str);
    LOG_INFO("basic_sync_set_cb end.");
}