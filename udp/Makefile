# Source files
SRCFILES	= $(wildcard *.cpp)
# Object files
OBJFILES	= $(patsubst %.cpp, %.o, $(SRCFILES))


# Default rule for .cpp files
%.o : %.cpp
	@echo "	CXX	$<"
	@$(CXX) -c $< -o $@ $(CXXFLAGS)


# PHONY targets
.PHONY : all clean


# Build Server & client
all : $(OBJFILES)

# Cleanup
clean :
	@echo "	RM	$(OBJFILES)"
	@rm -rf $(OBJFILES)

 
