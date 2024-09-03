# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/overflow: overflow.c
	@mkdir -p build
	gcc overflow.c -Wall -Werror -o build/overflow

run: build/overflow
	@./build/overflow

clean:
	@rm -rf build

.PHONY: run clean
