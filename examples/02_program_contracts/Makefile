# No copyright. Vladislav Aleinik, 2023

build/adder: adder.c
	@mkdir -p build
	gcc adder.c -std=c99 -Wall -Werror -o build/adder

run: build/adder
	@./build/adder

clean:
	@rm -rf build

.PHONY: run clean
