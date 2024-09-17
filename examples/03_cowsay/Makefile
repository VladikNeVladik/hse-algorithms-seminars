# Copyright 2024 Vladislav Aleinik

build/cowsay: cowsay.c
	@mkdir -p build
	gcc cowsay.c -std=c99 -Wall -Werror -o build/cowsay

run: build/cowsay
	@./build/cowsay

clean:
	@rm -rf build

.PHONY: run clean
