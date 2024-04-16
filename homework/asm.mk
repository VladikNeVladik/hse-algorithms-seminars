#===========#
# Toolchain #
#===========#

CC  = x86_64-linux-gnu-gcc
AS  = nasm
GDB = gdb

#=======================#
# Compiler/linker flags #
#=======================#

CFLAGS = \
	-m32 \
	-Wfatal-errors \
	-no-pie \
	-g

ASFLAGS = \
	-f elf32 \
	-DUNIX \
	-g

LDFLAGS = \
	-m32 \
	-no-pie \
	-z noexecstack \
	-g

#========#
# Colors #
#========#

BRED    = \033[1;31m
BGREEN  = \033[1;32m
BYELLOW = \033[1;33m
GREEN   = \033[1;35m
BCYAN   = \033[1;36m
RESET   = \033[0m

#===================#
# Command arguments #
#===================#

PROGRAM_BIN=build/$(PROGRAM)

default: verify-input-program $(PROGRAM_BIN)

# Command arguments:
# PROGRAM - program being tested

verify-input-program:
	@if [ -z "$(PROGRAM)" ]; then \
		printf "$(BRED)Error$(BYELLOW): define program as $(BCYAN)PROGRAM=program$(RESET)\n"; \
		exit 1; \
	fi

#=======#
# Files #
#=======#

SOURCES = \
	../../macro.c \
	$(PROGRAM).asm

OBJECTS = \
	build/macro.o \
	build/$(PROGRAM).o

EXECUTABLE = build/cond_br

#---------------
# Build scripts
#---------------

$(PROGRAM_BIN): $(OBJECTS) Makefile
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

build/macro.o: ../../macro.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.asm
	@mkdir -p build
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf build

#----------------------
# Emulator interaction
#----------------------

GDB_FLAGS = \
	--eval-command="set architecture i386" \
	--eval-command="file $(PROGRAM_BIN)" \
	--eval-command="b main" \
	--eval-command="run" \
	--eval-command="tui e" \
	--eval-command="la regs"

gdb: $(PROGRAM_BIN)
	$(GDB) $(GDB_FLAGS)

.PHONY: all clean gdb
