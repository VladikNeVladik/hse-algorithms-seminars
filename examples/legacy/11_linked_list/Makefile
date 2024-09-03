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

INCLUDES=\
	list.h \
	polynomial.h \
	utils.h

build/read: read.c $(INCLUDES)
	@mkdir -p build
	gcc read.c ${CFLAGS} -o build/read

build/write: write.c $(INCLUDES)
	@mkdir -p build
	gcc write.c ${CFLAGS} -o build/write

read: build/read
	@mkdir -p res
	@./build/read

write: build/write
	@mkdir -p res
	@./build/write

clean:
	@rm -rf res
	@rm -rf build

.PHONY: read write clean
