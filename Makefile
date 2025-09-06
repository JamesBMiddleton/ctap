SHELL = /bin/sh

.SILENT:

.PHONY: build check analyze clean

LIB_NAME := ctap
SRC_DIR := src
BUILD_DIR := build
UTEST_DIR := test/unit
ITEST_DIR := test/integration
UTILTEST_DIR := test/utilities

CC := clang-19
AR := ar
CFLAGS := -std=c99 -g -I. 
RELEASE_CFLAGS := -O3 -flto -finline-functions \
				  -ffunction-sections -fdata-sections \
				  -ffreestanding \
				  $(CFLAGS)
DEBUG_CFLAGS := -DDEBUG -O0 -Weverything -Werror -fsanitize=address \
				-fsanitize=undefined -fno-omit-frame-pointer \
				-fstack-protector-strong -fno-inline \
				-Wno-declaration-after-statement \
				-Wno-padded \
				-Wno-unsafe-buffer-usage \
				-Wno-switch-default \
				-Wno-empty-translation-unit \
				$(CFLAGS)

SRCS := $(shell find $(SRC_DIR) -name '*.c' -type f)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
ITESTS := $(shell find $(ITEST_DIR) -name '*.c' -type f)
UTESTS := $(shell find $(UTEST_DIR) -name '*.c' -type f)
UTIL_TESTS := $(shell find $(UTILTEST_DIR) -name '*.c' -type f)
UTEST_BINS := $(UTESTS:%.c=$(BUILD_DIR)/%)
UTEST_LOGS := $(patsubst %,%.log,$(UTEST_BINS))
ITEST_BINS := $(ITESTS:%.c=$(BUILD_DIR)/%)
ITEST_LOGS := $(patsubst %,%.log,$(ITEST_BINS))
UTILTEST_BINS := $(UTIL_TESTS:%.c=$(BUILD_DIR)/%)
UTILTEST_LOGS := $(patsubst %,%.log,$(UTILTEST_BINS))

build: CFLAGS = $(RELEASE_CFLAGS)
build: $(BUILD_DIR)/$(LIB_NAME).a

check: CFLAGS = $(RELEASE_CFLAGS)
check: $(UTEST_LOGS) $(ITEST_LOGS) $(UTILTEST_LOGS)

$(BUILD_DIR)/$(SRC_DIR):
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)
$(BUILD_DIR)/$(UTEST_DIR):
	mkdir -p $(BUILD_DIR)/$(UTEST_DIR)
$(BUILD_DIR)/$(ITEST_DIR):
	mkdir -p $(BUILD_DIR)/$(ITEST_DIR)
$(BUILD_DIR)/$(UTILTEST_DIR):
	mkdir -p $(BUILD_DIR)/$(UTILTEST_DIR)

# Create a static library archive.
$(BUILD_DIR)/$(LIB_NAME).a: $(BUILD_DIR)/$(SRC_DIR) $(OBJS) 
	$(AR) rcs $(BUILD_DIR)/$(LIB_NAME).a $(OBJS)

# Build object files.
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Run the util test binaries, generating the test logs.
$(UTILTEST_LOGS): $(BUILD_DIR)/$(UTILTEST_DIR) $(UTILTEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the integration test binaries, generating the test logs.
$(ITEST_LOGS): $(BUILD_DIR)/$(ITEST_DIR) $(ITEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the unit test binaries, generating the test logs.
$(UTEST_LOGS): $(BUILD_DIR)/$(UTEST_DIR) $(UTEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Build util test binaries
$(BUILD_DIR)/$(UTILTEST_DIR)/%: $(UTILTEST_DIR)/%.c
	$(CC) $< -o $@ $(CFLAGS)

# Build unit test binaries; test/unit/ctp_<module>_test.c + src/ctp_<module>_*.c 
$(BUILD_DIR)/$(UTEST_DIR)/%: $(UTEST_DIR)/%.c
	$(CC) $< $(patsubst $(UTEST_DIR)/%_test.c,$(SRC_DIR)/%*.c,$<) -o $@ $(CFLAGS)

# Build integration test binaries.
$(BUILD_DIR)/$(ITEST_DIR)/%: $(ITEST_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME).a
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	-rm -r $(BUILD_DIR)

