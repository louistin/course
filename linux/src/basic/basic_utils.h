#ifndef __BASIC_UTILS_H__
#define __BASIC_UTILS_H__

#define LOG_TAG                         "basic"
extern "C" {
#include "elog.h"
}

#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
using namespace std;

// epoll server
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <signal.h>



#define EPOLL_MAXFDS                            256
#define EPOLL_EVENTS                            64
#define EPOLL_SERVER_PORT                       1080
#define EPOLL_CLIENT_PORT                       1920

#endif