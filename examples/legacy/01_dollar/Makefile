# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/dollar: dollar.c
	@mkdir -p build
	gcc dollar.c -Wall -Werror -o build/dollar

run: build/dollar
	@./build/dollar

clean:
	@rm -rf build

.PHONY: run clean
