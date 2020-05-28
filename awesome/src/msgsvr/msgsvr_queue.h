#ifndef _MSGSVR_QUEUE_H
#define _MSGSVR_QUEUE_H

#include "msgsvr_util.h"

extern int msgsvr_queue_build(void **handler, int size);

extern int msgsvr_queue_free(void *handler);

extern int msgsvr_queue_read(void *handler, msgsvr_message_t *message);

extern int msgsvr_queue_write(void *handler, msgsvr_message_t *message);

#endif