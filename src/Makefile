# Set default parallel build
MAKEFLAGS += -j16

BIN=../makea1rom
# napple1 makefile 

CC = g++
CFLAGS = -Wall -Werror --std=c++23
DEBUG_FLAGS = -g
LIBS = -lpthread

# List of source files
SRC := $(wildcard *.cpp)

# Generate object files list from source files
OBJ = $(SRC:.cpp=.o)

# Generate dependency files list from source files
DEP = $(SRC:.cpp=.d)

all: $(BIN)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Generic rule for compiling .cpp files to .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

# Include dependency files
-include $(DEP)

clean:
	-rm -f $(OBJ)
	-rm -f $(DEP)
	-rm -f $(BIN)

test: $(BIN)
	./$(BIN) ../test.romspec
