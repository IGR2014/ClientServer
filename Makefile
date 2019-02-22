# Compiler
CXX		= g++
# Linker
LD		= g++

# Compiler flags
CXX_FLAGS	= -std=c++11 -s -W -Werror -pedantic -Os
# Linker flags
LD_FLAGS	= -pthread

# Source files
SRCFILES	= $(wildcard *.cpp)
# Object files
OBJFILES	= $(patsubst %.cpp, %.o, $(SRCFILES))


# Default rule for .cpp files
%.o : %.cpp
	@echo "	CXX	$<"
	@$(CXX) -c $< -o $@ $(CXX_FLAGS) $(LD_FLAGS)


# PHONY targets
.PHONY : all clean
	

# Build Server & client
all : $(OBJFILES)
	@echo "	MAKE	tcp"
	@$(MAKE) -C tcp
	@echo "	LD	tcpserver"
	@$(LD) example-tcpserver.o socket.o tcp/tcpserver.o -o tcpserver $(LD_FLAGS)
	@echo "	LD	tcpclient"
	@$(LD) example-tcpclient.o socket.o tcp/tcpclient.o -o tcpclient $(LD_FLAGS)

# Cleanup
clean :
	@echo "	RM	$(OBJFILES)"
	@rm -rf $(OBJFILES)
	@echo "	CLEAN	tcp"
	@$(MAKE) -C tcp clean
 
