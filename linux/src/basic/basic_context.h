#ifndef __BASIC_CONTEXT_H__
#define __BASIC_CONTEXT_H__

#include "basic_utils.h"

typedef struct {
    void *sync;
    void *async;
} basic_context_t;

#endif