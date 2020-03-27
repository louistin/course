#ifndef __BASIC_ASYNC_CALLBACK_DEFINE__
#define __BASIC_ASYNC_CALLBACK_DEFINE__

#include "../basic_utils.h"

typedef void (*fun_cb)(int num, const char *str);
void basic_async_set_cb(int num, const char *str, fun_cb callback);

#endif