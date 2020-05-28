#include "msgsvr_queue.h"
#include <queue>

typedef struct {
  std::queue<void *> *queue;
  unsigned int size;
} msgsvr_queue_t;

int msgsvr_queue_build(void **handler, int size) {
  msgsvr_queue_t *queue = (msgsvr_queue_t *)malloc(sizeof(msgsvr_queue_t));
  queue->queue = new std::queue<void *>;
  queue->size = size;

  *handler = queue;
  return OK;
}

int msgsvr_queue_free(void *handler) {
  msgsvr_queue_t *queue = (msgsvr_queue_t *)handler;
  msgsvr_message_t *message = NULL;
  while (queue->queue->empty() == 0) {
    message = (msgsvr_message_t *)queue->queue->front();
    if (message != NULL) {
      free(message);
      message = NULL;
    }

    queue->queue->pop();
  }

  delete(queue->queue);
  queue->queue = NULL;

  free(queue);
  queue = NULL;

  return OK;
}

int msgsvr_queue_read(void *handler, msgsvr_message_t *message) {
  msgsvr_queue_t *queue = (msgsvr_queue_t *)handler;
  msgsvr_message_t *__message = NULL;
  if (queue->queue->empty() == 0) {
    __message = (msgsvr_message_t *)queue->queue->front();
    queue->queue->pop();
  } else {
    LOG_ERROR("msgsvr queue is empty.\n");
    return ERROR;
  }

  if (__message == NULL) {
    LOG_ERROR("msgsvr queue __message is NULL.\n");
    return ERROR;
  }

  memcpy(message, __message, sizeof(msgsvr_message_t));
  free(__message);
  __message = NULL;

  return OK;
}

int msgsvr_queue_write(void *handler, msgsvr_message_t *message) {
  msgsvr_queue_t *queue = (msgsvr_queue_t *)handler;

  if (queue->queue->size() >= queue->size) {
    LOG_ERROR("msgsvr queue is full.\n");
    return ERROR;
  }

  msgsvr_message_t *__message = (msgsvr_message_t *)malloc(sizeof(msgsvr_message_t));
  memcpy(__message, message, sizeof(msgsvr_message_t));

  // TODO: 是否需要添加检查 push 操作是否成功的代码?
  queue->queue->push(__message);

  return OK;
}