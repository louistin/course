#ifndef __BASIC_UTILS_H__
#define __BASIC_UTILS_H__

#define LOG_TAG                         "basic"
#include "elog.h"

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>

// epoll server
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>

using namespace std;

#define EPOLL_MAXFDS                    256
#define EPOLL_EVENTS                    64
#define EPOLL_PORT                      1080

#endif