PLATFORM		?= x64
CURRENT_PATH	= $(shell pwd)/..
FRAMEWORK_PATH	= $(CURRENT_PATH)/framework/
THREAD_PATH		= $(FRAMEWORK_PATH)/linux/$(PLATFORM)
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
INCLUDE			= -I$(THREAD_PATH)/easylogger/include

THREAD_LIBS		= $(THREAD_PATH)/easylogger/lib/libeasylogger.a

CXXFLAGS		= -g $(DEBUG) $(WARN) $(OPTIMIZE) -fPIC $(DEFS) $(INCLUDE)
CFLAGS			= -g $(DEBUG) $(WARN) $(OPTIMIZE) -fPIC $(DEFS) $(INCLUDE)
LDFLAGS 		= -fPIC -o3
LIBS			= $(THREAD_LIBS)

BASIC_CALLBACK_OBJS	= 	$(SRC_PATH)/basic/basic.o 							\
					$(SRC_PATH)/basic/callback/basic_sync_callback.o 		\
					$(SRC_PATH)/basic/callback/basic_sync_callback_define.o	\
					$(SRC_PATH)/basic/callback/basic_async_callback.o		\
					$(SRC_PATH)/basic/callback/basic_async_callback_define.o\

BASIC_IO_EPOLL_SERVER_OBJS	= 	$(SRC_PATH)/basic/basic.o 					\
								$(SRC_PATH)/basic/io/basic_epoll_server.o	\

BASIC_IO_EPOLL_CLIENT_OBJS	= 	$(SRC_PATH)/basic/basic.o 					\
								$(SRC_PATH)/basic/io/basic_epoll_client.o	\

all:
	$(MAKE) -f makefile.basic_callback CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" BUILD_PATH="$(BUILD_PATH)" THREAD_PATH="$(THREAD_PATH)" FRAMEWORK_PATH="$(FRAMEWORK_PATH)" LIBS="$(LIBS)" SRC_PATH="$(SRC_PATH)" BASIC_OBJS="$(BASIC_OBJS)"

clean:
	$(MAKE) -f makefile.basic_callback clean CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" BUILD_PATH="$(BUILD_PATH)" THREAD_PATH="$(THREAD_PATH)" FRAMEWORK_PATH="$(FRAMEWORK_PATH)" LIBS="$(LIBS)" LDFLAGS="$(LDFLAGS)" SRC_PATH="$(SRC_PATH)"
