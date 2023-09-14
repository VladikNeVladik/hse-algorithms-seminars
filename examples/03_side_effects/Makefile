# No copyright. Vladislav Aleinik, 2023
# The simplest Makefile possible.

build/cond: cond.c
	@mkdir -p build
	gcc cond.c -Wall -Werror -o build/cond

run: build/cond
	@./build/cond

clean:
	@rm -rf build

.PHONY: run clean
