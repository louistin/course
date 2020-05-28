PLATFORM		?= x64
CURRENT_PATH	= $(shell pwd)/..
FRAMEWORK_PATH	= $(CURRENT_PATH)/framework/
THREAD_PATH		= $(FRAMEWORK_PATH)
BUILD_PATH		= $(CURRENT_PATH)/build
SRC_PATH		= $(CURRENT_PATH)/src

CC				= $(PREFIX)gcc
CXX				= $(PREFIX)g++ -std=c++11
AR 				= $(PREFIX)ar
LD				= $(PREFIX)ld

DEBUG			=
OPTIMIZE 		=
WARN 			= -Wall
DEFS 			=
INCLUDE			= -I$(THREAD_PATH)/easylogger/include				\
							-I$(THREAD_PATH)/libevent/include

THREAD_LIBS		= $(THREAD_PATH)/easylogger/lib/libeasylogger.a		\
								$(THREAD_PATH)/libevent/lib/libevent.a

CXXFLAGS		= -g $(DEBUG) $(WARN) $(OPTIMIZE) -fPIC $(DEFS) $(INCLUDE)
CFLAGS			= -g $(DEBUG) $(WARN) $(OPTIMIZE) -fPIC $(DEFS) $(INCLUDE)
LDFLAGS 		= -fPIC -o3
LIBS			= $(THREAD_LIBS)

MSGSVR_OBJS	= 	$(SRC_PATH)/msgsvr/msgsvr.o		\
												$(SRC_PATH)/msgsvr/msgsvr_queue.o		\
												$(SRC_PATH)/msgsvr/msgsvr_server.o	\
												$(SRC_PATH)/msgsvr/msgsvr_tcpserver.o		\
												$(SRC_PATH)/msgsvr/msgsvr_util.o		\

all:
	$(MAKE) -f makefile.msgsvr CXX="$(CXX) " CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" BUILD_PATH="$(BUILD_PATH)" THREAD_PATH="$(THREAD_PATH)" FRAMEWORK_PATH="$(FRAMEWORK_PATH)" LIBS="$(LIBS)" SRC_PATH="$(SRC_PATH)" MSGSVR_OBJS="$(MSGSVR_OBJS)"

clean:
	$(MAKE) -f makefile.msgsvr clean CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" BUILD_PATH="$(BUILD_PATH)" THREAD_PATH="$(THREAD_PATH)" FRAMEWORK_PATH="$(FRAMEWORK_PATH)" LIBS="$(LIBS)" LDFLAGS="$(LDFLAGS)" SRC_PATH="$(SRC_PATH)" MSGSVR_OBJS="$(MSGSVR_OBJS)"
