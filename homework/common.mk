# No copyright. Vladislav Aleinik, 2023

# Compilation flags:
CFLAGS=                          \
	-O2                          \
	-Wall                        \
	-Werror                      \
	-Wformat-security            \
	-Wignored-qualifiers         \
	-Winit-self                  \
	-Wswitch-default             \
	-Wfloat-equal                \
	-Wpointer-arith              \
	-Wtype-limits                \
	-Wempty-body                 \
	-Wno-logical-op              \
	-Wstrict-prototypes          \
	-Wold-style-declaration      \
	-Wold-style-definition       \
	-Wmissing-parameter-type     \
	-Wmissing-field-initializers \
	-Wnested-externs             \
	-Wno-pointer-sign            \
	-Wno-unused-result           \
	-std=gnu99                   \
	-lm

# Timing command usage:
TIME_CMD=/usr/bin/time
TIME_FORMAT="CPU Percentage: %P\nMemory: %t kB\nTime: %e sec\nReturn value: %x"

# Command arguments:
# PROGRAM - program being tested
PROGRAM_BIN=build/$(PROGRAM)

#=======================#
# Hand-made test system #
#=======================#

TEST_OUTPUTS = $(TESTS:%=build/%.output)
TIME_OUTPUT = build/time.output

test: $(PROGRAM_BIN) $(TEST_OUTPUTS) FORCE

build/%.output: tests/%.input $(PROGRAM_BIN) FORCE
	@mkdir -p tests
	@printf "\033[1;33mRunning \033[1;36m$*\033[1;33m for solution \033[1;36m$(PROGRAM)\033[0m\n"
	@$(TIME_CMD) --quiet --format=$(TIME_FORMAT) $(PROGRAM_BIN) < $< > $@ | cat
	@printf "\033[1;35m"
	@cat $@
	@printf "\033[0m"

time: $(PROGRAM_BIN) FORCE
	@printf "\033[1;33mRunning program \033[1;36m$(PROGRAM)\033[0m\n"
	@$(TIME_CMD) --quiet --format=$(TIME_FORMAT) $(PROGRAM_BIN) > ${TIME_OUTPUT} | cat
	@printf "\033[1;35m"
	@cat ${TIME_OUTPUT}
	@printf "\033[0m"

#==============#
# Build system #
#==============#

build/%: %.c
	@mkdir -p build
	@printf "\033[1;33mBuilding solution \033[1;36m$*\033[0m\n"
	@gcc $< $(CFLAGS) -o $@

clean: FORCE
	@rm -rf build
	@rm -f tests/*-gen.input

.PHONY: test clean FORCE
