# Compiler and Flags
CC = g++
CFLAGS = -Wall -g
CFLAGS += -fsanitize=address -static-libasan 

# Source and Build Directories
SRCDIR = src
BUILDDIR = obj

# Target name
TARGET = $(BUILDDIR)/a.out

# Libraries
#LIBS = -lgvc -lcgraph

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

# Build object files from source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# clean up build directory
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
	rm *.dot
	rm *.png
