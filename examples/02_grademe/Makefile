# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/grademe: grademe.c
	@mkdir -p build
	gcc grademe.c -Wall -Werror -o build/grademe -lm

run: build/grademe
	@./build/grademe

clean:
	@rm -rf build

.PHONY: run clean
