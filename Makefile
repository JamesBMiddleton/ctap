SHELL = /bin/sh

.SILENT:

.PHONY: build check analyze clean

LIB_NAME := ctap
SRC_DIR := src
BUILD_DIR := build
UTEST_DIR := test/unit
ITEST_DIR := test/integration

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
ITESTS := $(shell find $(ITEST_DIR) -name '*.c' -type f)
UTESTS := $(shell find $(UTEST_DIR) -name '*.c' -type f)
UTEST_BINS := $(UTESTS:%.c=$(BUILD_DIR)/%)
UTEST_LOGS := $(patsubst %,%.log,$(UTEST_BINS))
ITEST_BINS := $(ITESTS:%.c=$(BUILD_DIR)/%)
ITEST_LOGS := $(patsubst %,%.log,$(ITEST_BINS))

build: CFLAGS = $(RELEASE_CFLAGS)
build: $(BUILD_DIR)/$(LIB_NAME).a

# Create a static library archive.
$(BUILD_DIR)/$(LIB_NAME).a: $(BUILD_DIR)/$(LIB_NAME).o
	$(AR) rcs $(BUILD_DIR)/$(LIB_NAME).a $(BUILD_DIR)/$(LIB_NAME).o

# Build library object.
$(BUILD_DIR)/$(LIB_NAME).o: $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/$(LIB_NAME).c -o $(BUILD_DIR)/$(LIB_NAME).o $(CFLAGS)

check: CFLAGS = $(RELEASE_CFLAGS)
check: $(UTEST_LOGS) $(ITEST_LOGS)

# Run the integration test binaries, generating the test logs.
$(ITEST_LOGS): $(ITEST_BINS)
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the unit test binaries, generating the test logs.
$(UTEST_LOGS): $(UTEST_BINS)
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Build unit test binaries.
$(BUILD_DIR)/$(UTEST_DIR)/%: $(UTEST_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $< -o $@ $(CFLAGS)

# Build intergation test binaries.
$(BUILD_DIR)/$(ITEST_DIR)/%: $(ITEST_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME).a
	mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	-rm -r $(BUILD_DIR)

