SHELL = /bin/sh

.SILENT:

.PHONY: dirs build check analyze clean

LIB_NAME := ctap
SRC_DIR := src
TEST_DIR := test
BUILD_DIR := build
UTEST_DIR := test/unit
ITEST_DIR := test/integration
UTILTEST_DIR := test/utilities

CC := clang
AR := ar rcs 
CFLAGS := -std=c89 -g -I. -MMD
RELEASE_CFLAGS := -O3 -flto -finline-functions \
				  -ffunction-sections -fdata-sections \
				  -ffreestanding \
				  -ferror-limit=1 \
				  $(CFLAGS)
DEBUG_CFLAGS := -DDEBUG -O0 -Weverything -Werror -fsanitize=address \
				-fsanitize=undefined -fno-omit-frame-pointer \
				-fstack-protector-strong -fno-inline \
				-ferror-limit=1 \
				-Wno-declaration-after-statement \
				-Wno-padded \
				-Wno-unsafe-buffer-usage \
				-Wno-switch-default \
				-Wno-empty-translation-unit \
				-Wno-unused-functions \
				$(CFLAGS)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)
ITESTS := $(wildcard $(ITEST_DIR)/*.c)
UTESTS := $(wildcard $(UTEST_DIR)/*.c)
UTILTESTS := $(wildcard $(UTILTEST_DIR)/*.c)
UTEST_BINS := $(UTESTS:%.c=$(BUILD_DIR)/%)
ITEST_BINS := $(ITESTS:%.c=$(BUILD_DIR)/%)
UTILTEST_BINS := $(UTILTESTS:%.c=$(BUILD_DIR)/%)
UTEST_LOGS := $(patsubst %,%.log,$(UTEST_BINS))
ITEST_LOGS := $(patsubst %,%.log,$(ITEST_BINS))
UTILTEST_LOGS := $(patsubst %,%.log,$(UTILTEST_BINS))

# Include header dependency targets if they've been generated with -MMD.
-include $(DEPS)

# build: CFLAGS = $(RELEASE_CFLAGS)
build: CFLAGS = $(DEBUG_CFLAGS)
build: dirs $(BUILD_DIR)/$(LIB_NAME).a

check: CFLAGS = $(RELEASE_CFLAGS)
check: dirs $(UTEST_LOGS) $(ITEST_LOGS) $(UTILTEST_LOGS)

dirs:
	test -d $(BUILD_DIR) || mkdir $(BUILD_DIR)
	test -d $(BUILD_DIR)/$(SRC_DIR) || mkdir $(BUILD_DIR)/$(SRC_DIR)
	test -d $(BUILD_DIR)/$(TEST_DIR) || mkdir $(BUILD_DIR)/$(TEST_DIR)
	test -d $(BUILD_DIR)/$(UTEST_DIR) || mkdir $(BUILD_DIR)/$(UTEST_DIR)
	test -d $(BUILD_DIR)/$(ITEST_DIR) || mkdir $(BUILD_DIR)/$(ITEST_DIR)
	test -d $(BUILD_DIR)/$(UTILTEST_DIR) || mkdir $(BUILD_DIR)/$(UTILTEST_DIR)

# Create a static library archive.
$(BUILD_DIR)/$(LIB_NAME).a: $(OBJS) 
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
	-test -d $(BUILD_DIR) && rm -r $(BUILD_DIR)
