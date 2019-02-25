# Source files
SRCFILES	= $(wildcard *.cpp)
# Object files
OBJFILES	= $(patsubst %.cpp, %.o, $(SRCFILES))

# TCP server target
SERVER		= tcpserver
# TCP client target
CLIENT		= tcpclient


# Default rule for .cpp files
%.o : %.cpp
	@echo "	CXX	$^"
	@$(CXX) -c $^ -o $@ $(CXXFLAGS)


# PHONY targets
.PHONY : all clean


# Build Server & client
all : $(SERVER) $(CLIENT)

# TCP server build rule
$(SERVER) : $(SERVER).o
	@echo "	LD	$(SERVER)"
	@$(LD) $^ -o $@ ../../tcp/$(SERVER).o ../../tcp/tcpsocket.o $(LDFLAGS)

# TCP client build rule
$(CLIENT) : $(CLIENT).o
	@echo "	LD	$(CLIENT)"
	@$(CXX) $^ -o $@ ../../tcp/$(CLIENT).o ../../tcp/tcpsocket.o $(CXXFLAGS) $(LDFLAGS)

# Cleanup
clean :
	@echo "	RM	$(OBJFILES)"
	@rm -rf $(OBJFILES)
	@echo "	RM	$(SERVER)"
	@rm -rf $(SERVER)
	@echo "	RM	$(CLIENT)"
	@rm -rf $(CLIENT)

 
