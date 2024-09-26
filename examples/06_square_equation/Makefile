# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/square: square.c
	@mkdir -p build
	gcc square.c -Wall -Werror -o build/square -lm

run: build/square
	@./build/square

clean:
	@rm -rf build

.PHONY: run clean
