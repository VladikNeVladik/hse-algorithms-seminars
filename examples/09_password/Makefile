# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

CFLAGS = \
	-Wall \
	-Werror \
	-fno-stack-protector

build/password: password.c
	@mkdir -p build
	@gcc password.c $(CFLAGS) -g -o build/password

run: build/password
	@./build/password

break: build/password
	printf 01234567890123456789012345678901ABCDEFGHIJK\\x01 | ./build/password

clean:
	@rm -rf build

.PHONY: run clean
