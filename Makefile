SHELL = /bin/sh

.SILENT: 

.PHONY: check analyze clean

BUILD_DIR := out/build
TEST_DIR := test
SRC_DIR := src

CC := clang-18
AR := ar
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
				-Wno-empty-translation-unit \
				$(CFLAGS)

# Find all source files we want to check for changes
SRCS := $(shell find $(SRC_DIR) -name '*.c')
# Find all tests we want to compile
TESTS := $(shell find $(TEST_DIR) -name '*.c')
# test object filepath = BUILD_DIR + test filepath + .o
TEST_OBJS := $(TESTS:%=$(BUILD_DIR)/%.o)
# test binary filepath = test object filepath minus .c.o
TEST_BINS = $(patsubst %.c.o,%,$(TEST_OBJS))
# test log filepath = test binary filepath + .log
TEST_LOGS = $(patsubst %.c.o,%.log,$(TEST_OBJS))

build: $(BUILD_DIR)/$(SRC_DIR)/lib.a

# Create a static library archive
$(BUILD_DIR)/$(SRC_DIR)/lib.a: $(BUILD_DIR)/$(SRC_DIR)/lib.o
	$(AR) rcs $(BUILD_DIR)/$(SRC_DIR)/lib.a $(BUILD_DIR)/$(SRC_DIR)/lib.o

# Build library object
$(BUILD_DIR)/$(SRC_DIR)/lib.o: $(SRCS)
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	$(CC) -c $(SRC_DIR)/lib.c -o $(BUILD_DIR)/$(SRC_DIR)/lib.o $(CFLAGS)

clean:
	-rm -r $(BUILD_DIR)

check: CFLAGS = $(DEBUG_CFLAGS)
check: $(TEST_LOGS)

$(TEST_LOGS): $(TEST_BINS)
	{ $(patsubst %.log,%,$@) > $@ 2>&1 && echo "PASS: $@"; } || echo "FAIL: $@"

# Link the library and build the test binaries
$(TEST_BINS): $(TEST_OBJS)
	$(CC) -o $@ "$@.c.o" $(CFLAGS) 

# Build test object files
$(BUILD_DIR)/$(TEST_DIR)/%.c.o: $(TEST_DIR)/%.c $(SRCS)
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS) 




# # analyze: CFLAGS = $(DEBUG_CFLAGS)
# # analyze:
# # 	+ scan-build-15 make check
# #
#

