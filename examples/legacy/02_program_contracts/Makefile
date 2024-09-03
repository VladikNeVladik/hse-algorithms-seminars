# Copyright 2024 Vladislav Aleinik

build/adder: adder.c
	@mkdir -p build
	gcc adder.c -std=c99 -Wall -Werror -o build/adder

run: build/adder
	@./build/adder

clean:
	@rm -rf build

.PHONY: run clean
