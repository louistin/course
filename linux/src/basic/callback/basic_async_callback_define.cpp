#include "basic_async_callback_define.h"

typedef struct {
    int num;
    char str[64];
    fun_cb callback;
} basic_async_callback_t;

void *basic_async_callback_thread(void *p) {
    basic_async_callback_t *param = (basic_async_callback_t *)p;
    sleep(1);
    param->callback(param->num, param->str);

    return NULL;
}

void basic_async_set_cb(int num, const char *str, fun_cb callback) {
    LOG_INFO("basic_async_set_cb start.");
    basic_async_callback_t *p = (basic_async_callback_t *)malloc(sizeof(basic_async_callback_t));
    memset(p, 0, sizeof(basic_async_callback_t));
    p->num = num;
    strcpy(p->str, str);    // 这里是不安全的
    p->callback = callback;

    pthread_t pid;
    pthread_create(&pid, NULL, basic_async_callback_thread, (void *)p);
    LOG_INFO("basic_async_set_cb end.");

    // 阻塞等待线程 pid 结束
    pthread_join(pid, NULL);
}