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

# Different build scenarios:
simple: sum.c
	@mkdir -p build
	@gcc sum.c ${CFLAGS} -DSUM_SIMPLE -o build/sum

generator: sum.c
	@mkdir -p build
	@gcc sum.c ${CFLAGS} -DSUM_GENERATOR -o build/sum

tail: sum.c
	@mkdir -p build
	@gcc sum.c ${CFLAGS} -DSUM_TAIL_RECURSION -o build/sum

cycle: sum.c
	@mkdir -p build
	@gcc sum.c ${CFLAGS} -DSUM_CYCLE -o build/sum

# Run scenarios:
run: build/sum
	@./build/sum

clean:
	@rm -rf build

.PHONY: run clean
