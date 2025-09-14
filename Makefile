SHELL = /bin/sh

.SILENT:

.PHONY: build check analyze clean

LIB_NAME := ctap
SRC_DIR := src
BUILD_DIR := build
UTEST_DIR := test/unit
ITEST_DIR := test/integration
UTILTEST_DIR := test/utilities

CC := clang
AR := ar rcs 
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

SRCS := $(wildcard $(SRC_DIR)/*.c)
ITESTS := $(wildcard $(ITEST_DIR)/*.c)
UTESTS := $(wildcard $(UTEST_DIR)/*.c)
UTILTESTS := $(wildcard $(UTILTEST_DIR)/*.c)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
UTEST_BINS := $(UTESTS:%.c=$(BUILD_DIR)/%)
ITEST_BINS := $(ITESTS:%.c=$(BUILD_DIR)/%)
UTILTEST_BINS := $(UTILTESTS:%.c=$(BUILD_DIR)/%)
UTEST_LOGS := $(patsubst %,%.log,$(UTEST_BINS))
ITEST_LOGS := $(patsubst %,%.log,$(ITEST_BINS))
UTILTEST_LOGS := $(patsubst %,%.log,$(UTILTEST_BINS))

build: CFLAGS = $(RELEASE_CFLAGS)
build: $(BUILD_DIR)/$(LIB_NAME).a

check: CFLAGS = $(RELEASE_CFLAGS)
check: $(BUILD_DIR)/$(UTILTEST_DIR) $(BUILD_DIR)/$(ITEST_DIR) $(BUILD_DIR)/$(UTEST_DIR) \
	   $(UTEST_LOGS) $(ITEST_LOGS) $(UTILTEST_LOGS) 

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
	$(AR) $(BUILD_DIR)/$(LIB_NAME).a $(OBJS)

# Build object files.
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Run the utilities test binaries, generating the test logs.
$(UTILTEST_LOGS): $(UTILTEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the integration test binaries, generating the test logs.
$(ITEST_LOGS): $(ITEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the unit test binaries, generating the test logs.
$(UTEST_LOGS): $(UTEST_BINS) 
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Build utilities test binaries.
$(BUILD_DIR)/$(UTILTEST_DIR)/%: $(UTILTEST_DIR)/%.c
	$(CC) $< -o $@ $(CFLAGS)

# Build integration test binaries.
$(BUILD_DIR)/$(ITEST_DIR)/%: $(ITEST_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME).a
	$(CC) $^ -o $@ $(CFLAGS)

# Build unit test binaries; test/unit/tap_<module>_test.c + src/tap_<module>_*.c 
$(BUILD_DIR)/$(UTEST_DIR)/%: $(UTEST_DIR)/%.c
	$(CC) $< $(patsubst $(UTEST_DIR)/%_test.c,$(SRC_DIR)/%*.c,$<) -o $@ $(CFLAGS)

clean:
	-rm -r $(BUILD_DIR)
