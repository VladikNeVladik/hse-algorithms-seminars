# Copyright 2024 Vladislav Aleinik

#===============================#
# List of tests for the problem #
#===============================#

# List of tests for current problem:
TESTS = \
	stack-overflow-gen \
	overflow

tests/stack-overflow-gen.input: FORCE
	@mkdir -p tests
	@seq 10000000 20000000 > $@

# Include common Makefile:
include ../../common.mk
