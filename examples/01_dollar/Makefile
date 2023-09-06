# No copyright. Vladislav Aleinik, 2023
# The simplest Makefile possible.

build/dollar: dollar.c
	@mkdir -p build
	gcc dollar.c -o build/dollar

run: build/dollar
	@./build/dollar

clean:
	@rm -rf build

.PHONY: run clean
