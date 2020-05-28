#ifndef _MSGSVR_UTIL_H
#define _MSGSVR_UTIL_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <queue>

#include "elog.h"
#include "elog_cfg.h"

#define MSGSVR_MESSAGE_BODY_MAX 1024

#define MSGSVR_FAMILY_IPV4				2
#define MSGSVR_FAMILY_IPV6				10

#define MSGSVR_SIZE_MODE				16
#define MSGSVR_SIZE_ACTION				16
#define MSGSVR_SIZE_NODE				16
#define MSGSVR_SIZE_METHOD				16
#define MSGSVR_SIZE_CMD					16
#define MSGSVR_SIZE_TYPE				16
#define MSGSVR_SIZE_TRANSPORT			8
#define MSGSVR_SIZE_DOMAIN				32
#define MSGSVR_SIZE_IP					40
#define MSGSVR_SIZE_UUID				20
#define MSGSVR_SIZE_TOKEN				8
#define MSGSVR_SIZE_PASSWORD			32
#define MSGSVR_SIZE_SERVICE				32
#define MSGSVR_SIZE_STATUS				16
#define MSGSVR_SIZE_RESULT				16
#define MSGSVR_SIZE_PROPERTY			128
#define MSGSVR_SIZE_PROVINCE			32
#define MSGSVR_SIZE_COUNTRY				32
#define MSGSVR_SIZE_CONTINENT			32
#define MSGSVR_SIZE_SESSION				32
#define MSGSVR_SIZE_NAME				32
#define MSGSVR_SIZE_ERRSTR				255
#define MSGSVR_SIZE_AUTH				32
#define MSGSVR_SIZE_VERSION				16
#define MSGSVR_SIZE_ID					64
#define MSGSVR_SIZE_AUTHCODE			160


#define OK 0
#define ERROR -1

typedef struct {
  char local_ip[MSGSVR_SIZE_IP];
  unsigned short local_port;
  char remote_ip[MSGSVR_SIZE_IP];
  unsigned short remote_port;
} msgsvr_address_t;

typedef struct {
  char node;
  char method;
  char cmd;
  char type;
} msgsvr_message_header_t;

typedef struct {
  msgsvr_message_header_t header;
  int body_length;
  char body[MSGSVR_MESSAGE_BODY_MAX];
  msgsvr_address_t address;
} msgsvr_message_t;

#endif