# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/sum: sum.c
	@mkdir -p build
	gcc sum.c -Wall -Werror -o build/sum

run: build/sum
	@./build/sum

clean:
	@rm -rf build

.PHONY: run clean
