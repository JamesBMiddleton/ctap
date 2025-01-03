SHELL = /bin/sh
# MAKEFLAGS += -j$(shell nproc)

.SILENT: 

.PHONY: check analyze format clean

LIB_HEADER_NAME := src/ctap.h
BUILD_DIR := out/build
TEST_DIR := test

CC := clang-18
CFLAGS := -std=c99 -MMD -MP -g -I. 
RELEASE_CFLAGS := -O3 -flto -finline-functions \
				  -ffunction-sections -fdata-sections \
				  $(CFLAGS)
DEBUG_CFLAGS := -DDEBUG -O0 -Weverything -Werror -fsanitize=address \
				-fsanitize=undefined -fno-omit-frame-pointer \
				-fstack-protector-strong -fno-inline \
				-Wno-declaration-after-statement \
				-Wno-padded \
				-Wno-unsafe-buffer-usage \
				-Wno-switch-default \
				$(CFLAGS)

# Find all tests we want to compile
TESTS := $(shell find $(TEST_DIR) -name '*.c')
# test object filepath = BUILD_DIR + test filepath + .o
TEST_OBJS := $(TESTS:%=$(BUILD_DIR)/%.o)
# test binary filepath = test object filepath minus .c.o
TEST_BINS = $(patsubst %.c.o,%,$(TEST_OBJS))
# test log filepath = test binary filepath + .log
TEST_LOGS = $(patsubst %.c.o,%.log,$(TEST_OBJS))

check: CFLAGS = $(DEBUG_CFLAGS)
check: $(TEST_LOGS)

$(TEST_LOGS): $(TEST_BINS)
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

analyze: CFLAGS = $(DEBUG_CFLAGS)
analyze:
	+ scan-build-15 make check

format: $(LIB_HEADER_NAME) $(TESTS)
	clang-format-15 --verbose -i $(LIB_HEADER_NAME) $(TESTS)

clean:
	-rm -r $(BUILD_DIR)

# Build the test binaries
$(TEST_BINS): $(TEST_OBJS)
	$(CC) -o $@ "$@.c.o" $(CFLAGS) 

# Build test object files
$(BUILD_DIR)/$(TEST_DIR)/%.c.o: $(TEST_DIR)/%.c $(LIB_HEADER_NAME)
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) 
