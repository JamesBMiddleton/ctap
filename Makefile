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
				-Wno-declaration-after-statement \
				$(CFLAGS)

# Find all tests we want to compile
TESTS := $(shell find $(TEST_DIR) -name '*.c')
# test object filepath = BUILD_DIR + test filepath + .o
TEST_OBJS := $(TESTS:%=$(BUILD_DIR)/%.o)
# test binary filepath = test object filepath minus .c.o
TEST_BINS = $(patsubst %.c.o,%,$(TEST_OBJS))
# test log filepath = test binary filepath + .log
TEST_LOGS = $(patsubst %.c.o,%.log,$(TEST_OBJS))


.PHONY: check
check: CFLAGS = $(DEBUG_CFLAGS)
check: $(TEST_LOGS)

$(TEST_LOGS): $(TEST_BINS)
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

.PHONY: analyze
analyze: CFLAGS = $(DEBUG_CFLAGS)
analyze:
	+ scan-build-15 make check

.PHONY: format
format: $(LIB_HEADER_NAME) $(TESTS)
	clang-format-15 --verbose -i $(LIB_HEADER_NAME) $(TESTS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Build the test binaries
$(TEST_BINS): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ "$@.c.o"

# Build test object files
$(BUILD_DIR)/$(TEST_DIR)/%.c.o: $(TEST_DIR)/%.c $(LIB_HEADER_NAME)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
