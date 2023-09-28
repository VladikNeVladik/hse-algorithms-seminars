# No copyright. Vladislav Aleinik, 2023

#===============================#
# List of tests for the problem #
#===============================#

# List of tests for current problem:
TESTS = \
	array-overflow-gen

tests/array-overflow-gen.input: FORCE
	@mkdir -p tests
	@seq 1000000 2000000 > $@

# Include common Makefile:
include ../../common.mk
