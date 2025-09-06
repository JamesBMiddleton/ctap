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
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
ITESTS := $(shell find $(ITEST_DIR) -name '*.c' -type f)
UTESTS := $(shell find $(UTEST_DIR) -name '*.c' -type f)
UTEST_BINS := $(UTESTS:%.c=$(BUILD_DIR)/%)
UTEST_LOGS := $(patsubst %,%.log,$(UTEST_BINS))
ITEST_BINS := $(ITESTS:%.c=$(BUILD_DIR)/%)
ITEST_LOGS := $(patsubst %,%.log,$(ITEST_BINS))

build: CFLAGS = $(RELEASE_CFLAGS)
build: $(BUILD_DIR)/$(LIB_NAME).a

check: CFLAGS = $(RELEASE_CFLAGS)
check: $(ITEST_LOGS)
# check: $(UTEST_LOGS) $(ITEST_LOGS)

$(BUILD_DIR)/$(SRC_DIR):
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)
$(BUILD_DIR)/$(UTEST_DIR):
	mkdir -p $(BUILD_DIR)/$(UTEST_DIR)
$(BUILD_DIR)/$(ITEST_DIR):
	mkdir -p $(BUILD_DIR)/$(ITEST_DIR)

# Create a static library archive.
$(BUILD_DIR)/$(LIB_NAME).a: $(BUILD_DIR)/$(SRC_DIR) $(OBJS) 
	$(AR) rcs $(BUILD_DIR)/$(LIB_NAME).a $(OBJS)

# Build library object.
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	echo $<
	$(CC) -c $< -o $@ $(CFLAGS)

# Run the integration test binaries, generating the test logs.
$(ITEST_LOGS): $(BUILD_DIR)/$(ITEST_DIR) $(ITEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the unit test binaries, generating the test logs.
$(UTEST_LOGS): $(BUILD_DIR)/$(UTEST_DIR) $(UTEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Build unit test binaries.
$(BUILD_DIR)/$(UTEST_DIR)/%: $(UTEST_DIR)/%.c
	$(CC) $< -o $@ $(CFLAGS)

# Build integration test binaries.
$(BUILD_DIR)/$(ITEST_DIR)/%: $(ITEST_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME).a
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	-rm -r $(BUILD_DIR)

