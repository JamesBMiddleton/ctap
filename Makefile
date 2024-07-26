SHELL = /bin/sh
MAKEFLAGS += -j$(shell nproc)

.SILENT:

LIB_HEADER_NAME := ctap.h
BUILD_DIR := out/build
TEST_DIR := test

CC := clang-15
CFLAGS := -std=c99 -MMD -MP -g -I. 
RELEASE_CFLAGS := -O3 -flto -finline-functions \
				  -ffunction-sections -fdata-sections \
				  $(CFLAGS)
DEBUG_CFLAGS := -DDEBUG -O0 -Weverything -Werror -fsanitize=address \
				-fsanitize=undefined -fno-omit-frame-pointer \
				-fstack-protector-strong -fno-inline \
				$(CFLAGS)

# Find all the C files we want to compile
TESTS := $(shell find $(TEST_DIR) -name '*.c')
# object filepath = BUILD_DIR + source filepath + .o
TEST_OBJS := $(TESTS:%=$(BUILD_DIR)/%.o)
# test binary = test object filepath minus .c.o
TEST_BINS = $(patsubst %.c.o,%,$(TEST_OBJS))

.PHONY: all
all: build

.PHONY: check
check: CFLAGS = $(DEBUG_CFLAGS)
check: $(TEST_BINS)
	./$<

.PHONY: format
format: $(SRCS) $(TESTS)
	clang-format-15 --verbose -i $(SRCS) $(TESTS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Build the test binaries
$(TEST_BINS): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $<

# Build test object files
$(BUILD_DIR)/$(TEST_DIR)/%.c.o: $(TEST_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

