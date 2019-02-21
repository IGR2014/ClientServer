# Compiler
CXX		= g++

# Compiler flags
CXX_FLAGS	= -std=c++11 -s -W -Werror -pedantic -Os
# Linker flags
LD_FLAGS	= -pthread

# Server binary name
SERVER		= server
# Client binary name
CLIENT		= client


# Default rule for .cpp files
% : %.cpp
	@echo "	CXX	$<"
	@$(CXX) $< -o $@ $(CXX_FLAGS) $(LD_FLAGS)


# PHONY targets
.PHONY : all clean


# Build Server & client
all : $(SERVER) $(CLIENT)

# Cleanup
clean :
	@echo "	RM	$(SERVER)"
	@echo "	RM	$(CLIENT)"
	@rm -rf $(SERVER) $(CLIENT)

 
