SHELL = /bin/sh

.SILENT: 

.PHONY: check analyze clean

LIB_NAME := ctap
SRC_DIR := src
BUILD_DIR := build

CC := clang-18
AR := ar
CFLAGS := -std=c99 -g -I. 
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
				-Wno-empty-translation-unit \
				$(CFLAGS)

SRCS := $(shell find $(SRC_DIR) -name '*.c' -type f)
UTEST_BINS := $(SRCS:%.c=$(BUILD_DIR)/%_utest)
UTEST_LOGS := $(patsubst %,%.log,$(UTEST_BINS))

# Ensure output directories exist.
$(shell mkdir -p $(BUILD_DIR) $(UTEST_DIR))

build: CFLAGS = $(RELEASE_CFLAGS)
build: $(BUILD_DIR)/$(LIB_NAME).a

# Create a static library archive
$(BUILD_DIR)/$(LIB_NAME).a: $(BUILD_DIR)/$(LIB_NAME).o
	$(AR) rcs $(BUILD_DIR)/$(LIB_NAME).a $(BUILD_DIR)/$(LIB_NAME).o

# Build library object
$(BUILD_DIR)/$(LIB_NAME).o: $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/$(LIB_NAME).c -o $(BUILD_DIR)/$(LIB_NAME).o $(CFLAGS)

check: CFLAGS = $(DEBUG_CFLAGS)
check: $(UTEST_LOGS)

# Run the utest binaries, generating the utest logs.
$(UTEST_LOGS): $(UTEST_BINS)
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Build utest binaries files, defining MODULE_UTEST for each.
$(BUILD_DIR)/$(SRC_DIR)/%_utest: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $< -o $@ $(CFLAGS) -D$(shell echo $(notdir $@) | tr '[:lower:]' '[:upper:]')

analyze: CFLAGS = $(DEBUG_CFLAGS)
analyze:
	+ scan-build-15 make check

clean:
	-rm -r $(BUILD_DIR)

