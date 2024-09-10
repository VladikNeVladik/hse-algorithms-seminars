# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/hello: hello.c
	@mkdir -p build
	gcc hello.c -Wall -Werror -o build/hello

run: build/hello
	@./build/hello

clean:
	@rm -rf build

.PHONY: run clean
