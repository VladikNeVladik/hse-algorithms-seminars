# Copyright 2024 Vladislav Aleinik

#===============================#
# List of tests for the problem #
#===============================#

# List of tests for current problem:
TESTS = \
	array-overflow-gen

tests/array-overflow-gen.input: FORCE
	@mkdir -p tests
	@seq 10000000 20000000 > $@

# Include common Makefile:
include ../../common.mk
