# Copyright 2024 Vladislav Aleinik
# The simplest Makefile possible.

build/typecast: typecast.c
	@mkdir -p build
	gcc typecast.c -Wall -Werror -o build/typecast

run: build/typecast
	@./build/typecast

clean:
	@rm -rf build

.PHONY: run clean
