# No copyright. Vladislav Alenik, 2024

# How to use this Makefile:
# 1. Place all you .cpp files into the src folder.
#    List them in the SOURCES file.
# 2. Place all you .hpp files into the include folder.
#    List them in the INCLUDES file.
# 3. Set EXECUTABLE variable to your program's name.
# 4. Build your program with "make".
# 5. Perform debug build with "make DEBUG=1" or "DEBUG=1 make".
# 6. Run your program with "make run".
# 7. Clean all build files with "make clean".
#
# ENJOY!

#-----------------------
# Compiler/linker flags
#-----------------------

CC = g++

# Compiler flags:
CFLAGS = \
	-std=c++17 \
	-Wall      \
	-Wextra    \
	-Werror

# Linker flags:
# NOTE: it is sometimes required to link to math library.
# LDFLAGS =

# Select build mode:
# NOTE: invoke with "DEBUG=1 make" or "make DEBUG=1".
ifeq ($(DEBUG),1)
	# Add default symbols:
	CFLAGS += -g
else
	# Enable link-time optimization:
	CFLAGS  += -flto
	LDFLAGS += -flto
endif

#--------
# Colors
#--------

# Use ANSI color codes:
BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#-------
# Files
#-------

INCLUDES = \
	include/helloworld.hpp

# Add "include" folder to header search path:
CFLAGS += -I $(abspath include)

# List of sources:
SOURCES = \
	hello.cpp \
	world.cpp

OBJECTS = $(SOURCES:%.cpp=build/%.o)

EXECUTABLE = build/helloworld

#---------------
# Build process
#---------------

# By default, build executable:
# NOTE: first target in the file is the default.
default: $(EXECUTABLE)

# Link all object files together to obtain a binary:
# NOTE: all object files will be built first.
$(EXECUTABLE): $(OBJECTS)
	@printf "$(BYELLOW)Linking executable $(BCYAN)$@$(RESET)\n"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Compile an object file:
# NOTE: all include files are passed as dependencies (this may be redundant)
build/%.o: src/%.cpp $(INCLUDES)
	@printf "$(BYELLOW)Building object file $(BCYAN)$@$(RESET)\n"
	@mkdir -p build
	$(CC) -c $< $(CFLAGS) -o $@

#--------------
# Test scripts
#--------------

# Run program:
run: $(EXECUTABLE)
	./$(EXECUTABLE)

#---------------
# Miscellaneous
#---------------

clean:
	@printf "$(BYELLOW)Cleaning build directory $(RESET)\n"
	rm -rf build

# List of non-file targets:
.PHONY: run clean default
