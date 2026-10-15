# ./cstruct/Makefile

CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -g
INCLUDES = -Iinclude
BUILD    = build

# Auto-discover sources
LIB_SRCS  = $(wildcard src/*.c)
HEADERS   = $(wildcard include/*.h)
TEST_SRCS = $(wildcard tests/*.c)
TEST_BINS = $(patsubst tests/%.c,$(BUILD)/%,$(TEST_SRCS))

.PHONY: all test clean

# Build every test binary
all: $(TEST_BINS)

# Each test links against the whole library
$(BUILD)/%: tests/%.c $(LIB_SRCS) $(HEADERS) | $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(LIB_SRCS) -o $@

$(BUILD):
	mkdir -p $@

# Run every test and bail on first failure
test: all
	@for t in $(TEST_BINS); do \
		echo "== $$t =="; \
		./$$t || exit 1; \
	done

clean:
	rm -rf $(BUILD)
