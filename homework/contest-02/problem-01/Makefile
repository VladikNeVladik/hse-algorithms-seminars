# No copyright. Vladislav Aleinik, 2023

#===============================#
# List of tests for the problem #
#===============================#

# List of tests for current problem:
TESTS = \
	stack-overflow-gen \
	end-sequence-gen

tests/stack-overflow-gen.input: FORCE
	@mkdir -p tests
	@seq 200000 400000 > $@

tests/end-sequence-gen.input: FORCE
	@mkdir -p tests
	@seq 100 200 > $@

# Include common Makefile:
include ../../common.mk
