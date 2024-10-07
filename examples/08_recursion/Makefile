# Copyright 2024 Vladislav Aleinik

# Compilation flags:
CFLAGS=\
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
	-Wstrict-prototypes          \
	-Wold-style-definition       \
	-Wmissing-field-initializers \
	-Wnested-externs             \
	-Wno-pointer-sign            \
	-Wno-unused-result           \
	-std=gnu99                   \
	-lm                          \
	-fstack-usage

# Different build scenarios:
cycle: fibs.c
	@mkdir -p build
	@gcc fibs.c ${CFLAGS} -DFIBS_CYCLE -o build/fibs

bad: fibs.c
	@mkdir -p build
	@gcc fibs.c ${CFLAGS} -DFIBS_BAD_RECURSION -o build/fibs

good: fibs.c
	@mkdir -p build
	@clang fibs.c ${CFLAGS} -DFIBS_GOOD_RECURSION -o build/fibs

# Run scenarios:
run: build/fibs
	@./build/fibs

clean:
	@rm -rf build

.PHONY: run clean
