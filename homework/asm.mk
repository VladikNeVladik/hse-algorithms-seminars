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

#=======================#
# Hand-made test system #
#=======================#

# Timing command usage:
TIME_CMD    = /usr/bin/time
TIME_FORMAT = \
	"CPU Percentage: %P\nReal time: %e sec\nUser time: %U sec\nMemory: %t kB\nReturn value: %x"

#===================#
# Command arguments #
#===================#

PROGRAM_BIN=build/$(PROGRAM)

default: verify-input-program $(PROGRAM_BIN)

# Command arguments:
# PROGRAM - program being tested
# MAX_MEMORY - memory limit for heap
ifndef MAX_MEMORY
MAX_MEMORY=64000000# 64MiB
endif

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

#=======================#
# Hand-made test system #
#=======================#

# Test verdicts:
VERDICT_INAPT = "$(BYELLOW)[INAPT]$(RESET)\n"
VERDICT_OK    = "$(BGREEN)[OK]$(RESET)\n"
VERDICT_ERR   = "$(BRED)[ERR]$(RESET)\n"

OUTPUTS_TEST   = $(TESTS:%=build/%.output.test)
OUTPUTS_TIME   = $(TESTS:%=build/%.output.time)
OUTPUTS_MEMORY = $(TESTS:%=build/%.output.memory)

test:   verify-input-program $(PROGRAM_BIN) $(OUTPUTS_TEST)   FORCE
time:   verify-input-program $(PROGRAM_BIN) $(OUTPUTS_TIME)   FORCE
memory: verify-input-program $(PROGRAM_BIN) $(OUTPUTS_MEMORY) FORCE

build/%.output.test: tests/%.input $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Run test $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM)$(RESET)\n"
	@$(PROGRAM_BIN) < $< > $@ | cat
	@if [ ! -f tests/$*.output ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.output $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

build/%.output.time: tests/%.input $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Measure time on $(BCYAN)$*$(BYELLOW) for solution $(BCYAN)$(PROGRAM)$(RESET)\n"
	@$(TIME_CMD) --quiet --format=$(TIME_FORMAT) $(PROGRAM_BIN) < $< > $@ | cat
	@if [ ! -f tests/$*.output ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.output $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

build/%.output.memory: tests/%.input $(PROGRAM_BIN) FORCE
	@mkdir -p build
	@printf "$(BYELLOW)Limit memory ($(BCYAN)$(MAX_MEMORY)$(BYELLOW))"\
	" on $(BCYAN)$*$(BYELLOW) for \033[1;36m$(PROGRAM)$(RESET)\n"
	@prlimit --data=$(MAX_MEMORY) $(PROGRAM_BIN) < $< > $@ | cat
	@if [ ! -f tests/$*.output ]; then \
		printf $(VERDICT_INAPT); \
	elif cmp -s tests/$*.output $@; then \
		printf $(VERDICT_OK); \
	else \
		printf $(VERDICT_ERR); \
	fi

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

.PHONY: all clean gdb FORCE
