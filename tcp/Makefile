# Compiler
CXX		= g++

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

# Cleanup
clean :
	@echo "	RM	$(OBJFILES)"
	@rm -rf $(OBJFILES)

 
