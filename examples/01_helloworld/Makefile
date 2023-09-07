# No copyright. Vladislav Aleinik, 2023
# The simplest Makefile possible.

build/hello: hello.c
	@mkdir -p build
	gcc hello.c -Wall -Werror -o build/hello

run: build/hello
	@./build/hello

clean:
	@rm -rf build

.PHONY: run clean
