# No copyright. Vladislav Aleinik, 2023

# Timing command usage:
TIME_CMD=/usr/bin/time
TIME_FORMAT="CPU Percentage: %P\nMemory: %t kB\nTime: %e sec\nReturn value: %x"

# List of tests for current problem:
TESTS = \
	stack-overflow-gen \
	overflow

# Command arguments:
# PROGRAM - program being tested
PROGRAM_BIN=build/$(PROGRAM)

#=======================#
# Hand-made test system #
#=======================#

TEST_OUTPUTS = $(TESTS:%=tests/%.output)

test: $(PROGRAM_BIN) $(TEST_OUTPUTS) FORCE

tests/%.output: tests/%.input $(PROGRAM_BIN) FORCE
	@mkdir -p tests
	@printf "\033[1;33mRunning \033[1;36m$*\033[1;33m for solution \033[1;36m$(PROGRAM)\033[0m\n"
	@$(TIME_CMD) --quiet --format=$(TIME_FORMAT) $(PROGRAM_BIN) < $< > $@ | cat
	@printf "\033[1;35m"
	@cat $@
	@printf "\033[0m"

tests/stack-overflow-gen.input: FORCE
	@mkdir -p tests
	@seq 10000000 20000000 > $@

#==============#
# Build system #
#==============#

build/%: %.c
	@mkdir -p build
	@printf "\033[1;33mBuilding solution \033[1;36m$@\033[0m\n"
	@gcc $< $(CFLAGS) -o $@

clean: FORCE
	@rm -rf build
	@rm tests/*-gen.input
	@rm tests/*.output

.PHONY: test clean FORCE
