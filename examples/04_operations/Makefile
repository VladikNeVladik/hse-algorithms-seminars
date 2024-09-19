# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/operations: operations.c
	@mkdir -p build
	gcc operations.c -Wall -Werror -o build/operations

run: build/operations
	@./build/operations

clean:
	@rm -rf build

.PHONY: run clean
