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

BASIC_OBJS		= 	$(SRC_PATH)/basic/basic.o 								\
					$(SRC_PATH)/basic/basic_string.o 						\
					$(SRC_PATH)/basic/basic_time.o 							\
					$(SRC_PATH)/basic/basic_reference.o

all:
	$(MAKE) -f makefile.basic CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" BUILD_PATH="$(BUILD_PATH)" THREAD_PATH="$(THREAD_PATH)" FRAMEWORK_PATH="$(FRAMEWORK_PATH)" LIBS="$(LIBS)" SRC_PATH="$(SRC_PATH)" BASIC_OBJS="$(BASIC_OBJS)"

clean:
	$(MAKE) -f makefile.basic clean CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" SRC_PATH="$(SRC_PATH)"
