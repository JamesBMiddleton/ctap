SHELL = /bin/sh
LIB_NAME := ctap
SRC_DIR := src
TEST_DIR := test
BUILD_DIR := build
UTEST_DIR := test/unit
ITEST_DIR := test/integration
UTILTEST_DIR := test/utilities

CC := clang
AR := ar rcs 
CFLAGS := -std=c89 -I.
SAN_FLAGS := -fsanitize=address -fsanitize=undefined 
RELEASE_FLAGS := -O3 -flto -finline-functions \
				  -ffunction-sections -fdata-sections \
				  -ffreestanding
DEBUG_FLAGS :=  -DTAP_DEBUG -O0 
TEST_FLAGS := 	-g3 -MMD -Weverything -Werror \
				-fno-omit-frame-pointer \
				-fstack-protector-strong -fno-inline \
				-ferror-limit=1 \
				-Wno-declaration-after-statement \
				-Wno-padded \
				-Wno-unsafe-buffer-usage \
				-Wno-switch-default \
				-Wno-empty-translation-unit \
				-Wno-poison-system-directories \
				-Wno-pre-c23-compat \
				-Wno-pre-c11-compat \
				-Wno-unused-function \
				-Wno-unused-local-typedef

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

UTEST_SRCS := $(wildcard $(UTEST_DIR)/*.c)
UTEST_BINS := $(UTEST_SRCS:%.c=$(BUILD_DIR)/%)
UTEST_LOGS := $(UTEST_BINS:%=%.log)

ITEST_SRCS := $(wildcard $(ITEST_DIR)/*.c)
ITEST_BINS := $(ITEST_SRCS:%.c=$(BUILD_DIR)/%)
ITEST_LOGS := $(ITEST_BINS:%=%.log)

UTILTEST_SRCS := $(wildcard $(UTILTEST_DIR)/*.c)
UTILTEST_BINS := $(UTILTEST_SRCS:%.c=$(BUILD_DIR)/%)
UTILTEST_LOGS := $(UTILTEST_BINS:%=%.log)

.SILENT:
.PHONY: dirs build check clean
.PRECIOUS: $(UTEST_BINS) $(UTILTEST_BINS) $(ITEST_BINS)

all: build

build: ALL_FLAGS = $(RELEASE_FLAGS) $(CFLAGS)
build: dirs $(BUILD_DIR)/$(LIB_NAME).a

check: ALL_FLAGS = $(RELEASE_FLAGS) $(TEST_FLAGS) $(SAN_FLAGS) $(CFLAGS)
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
	$(CC) -c $< -o $@ $(ALL_FLAGS)

# Run the utilities test binaries, generating the test logs.
$(BUILD_DIR)/$(UTILTEST_DIR)/%.log: $(BUILD_DIR)/$(UTILTEST_DIR)/% 
	{ $< > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the integration test binaries, generating the test logs.
$(BUILD_DIR)/$(ITEST_DIR)/%.log: $(BUILD_DIR)/$(ITEST_DIR)/%
	{ $< > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Run the unit test binaries, generating the test logs.
$(BUILD_DIR)/$(UTEST_DIR)/%.log: $(BUILD_DIR)/$(UTEST_DIR)/% 
	{ $< > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Build utilities test binaries.
$(BUILD_DIR)/$(UTILTEST_DIR)/%: $(UTILTEST_DIR)/%.c
	$(CC) $< -o $@ $(ALL_FLAGS)

# Build integration test binaries.
$(BUILD_DIR)/$(ITEST_DIR)/%: $(ITEST_DIR)/%.c $(BUILD_DIR)/$(LIB_NAME).a
	$(CC) $< $(BUILD_DIR)/$(LIB_NAME).a -o $@ $(ALL_FLAGS)

# Build unit test binaries; test/unit/tap_<module>_test.c + src/tap_<module>_*.c 
$(BUILD_DIR)/$(UTEST_DIR)/%: $(UTEST_DIR)/%.c
	$(CC) $< $(patsubst $(UTEST_DIR)/%_test.c,$(SRC_DIR)/%*.c,$<) -o $@ $(ALL_FLAGS)

clean:
	-test -d $(BUILD_DIR) && rm -r $(BUILD_DIR)

# Include header dependency targets if they've been generated with -MMD.
-include $(OBJS:.o=.d)
-include $(UTEST_BINS:%=%.d)
-include $(ITEST_BINS:%=%.d)
-include $(UTILTEST_BINS:%=%.d)
