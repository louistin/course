BIN				= cp1
SRC				= $(SRC_PATH)

STATIC_LIBS		= $(LIBS)
SHARED_LIBS		= -lpthread
SHARED_DIR		=

OBJS			=	$(CP_OBJS)									\
					$(SRC)/cp/cp.o

all: $(BUILD_PATH)/$(BIN)

$(BUILD_PATH)/$(BIN):$(OBJS)
	$(CXX) $(LDFLAGS) $(SHARED_DIR) $(OBJS) $(STATIC_LIBS) -o $(BUILD_PATH)/$(BIN) $(SHARED_LIBS)
%.o:%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(BUILD_PATH)/$(BIN)
	rm -rf $(SRC)/cp/*.o
