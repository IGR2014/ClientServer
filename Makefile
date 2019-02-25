# Compiler
export CXX		= g++
# Linker
export LD		= g++

# Compiler flags
export CXXFLAGS		= -std=c++11 -s -W -Werror -Wall -pedantic -Os -pthread -I../../.
# Linker flags
export LDFLAGS		= -lpthread -lstdc++ -lc

# Source files
SRCFILES		= $(wildcard *.cpp)
# Object files
OBJFILES		= $(patsubst %.cpp, %.o, $(SRCFILES))


# Default rule for .cpp files
%.o : %.cpp
	@echo "	CXX	$^"
	@$(CXX) -c $^ -o $@ $(CXXFLAGS)


# PHONY targets
.PHONY : all clean


# Build Server & client
all : $(OBJFILES)
	@echo "	MAKE	tcp"
	@$(MAKE) -C tcp
	@echo "	MAKE	udp"
	@$(MAKE) -C udp
	@echo "	MAKE	tests"
	@$(MAKE) -C tests


# Cleanup
clean :
	@echo "	RM	$(OBJFILES)"
	@rm -rf $(OBJFILES)
	@echo "	CLEAN	tests"
	@$(MAKE) -C tests clean
	@echo "	CLEAN	udp"
	@$(MAKE) -C udp clean
	@echo "	CLEAN	tcp"
	@$(MAKE) -C tcp clean


