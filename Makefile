SHELL = /bin/sh
MAKEFLAGS += -j$(shell nproc)

.SILENT:

AR := ar
CC := clang-15

CFLAGS := -std=c17 -MMD -MP -g
LDFLAGS :=

LIB_NAME := ctap
BUILD_DIR := out/build
SRC_DIR := src
TEST_DIR := test

RELEASE_CFLAGS := -O3 -flto -finline-functions \
				  -ffunction-sections -fdata-sections -Wl,--gc-sections \
				  $(CFLAGS)
DEBUG_CFLAGS := -O0 -Weverything -fsanitize=address -fsanitize=undefined \
				-fno-omit-frame-pointer -fstack-protector-strong \
				-fno-inline \
				$(CFLAGS)

# Find all the C files we want to compile
SRCS := $(shell find $(SRC_DIR) -name '*.c')
TESTS := $(shell find $(TEST_DIR) -name '*.c')

# object filepath = BUILD_DIR + source filepath + .o
SRC_OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TESTS:%=$(BUILD_DIR)/%.o)
LIB_OBJ = $(BUILD_DIR)/lib$(LIB_NAME).a

# test binary = test object filepath minus .c.o
TEST_BINS = $(patsubst %.c.o,%,$(TEST_OBJS))

all: build

build: CFLAGS = $(RELEASE_CFLAGS)
build: $(LIB_OBJ)

check: CFLAGS = $(DEBUG_CFLAGS)
check: $(TEST_BINS)
	./$<

# Build the test binaries
$(TEST_BINS): $(TEST_OBJS) $(LIB_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< -L$(BUILD_DIR) -l$(LIB_NAME)

# Build the static library
$(LIB_OBJ): $(SRC_OBJS)
	$(AR) rcs $(LIB_OBJ) $(SRC_OBJS)

# Build src object files
$(BUILD_DIR)/$(SRC_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build test object files
$(BUILD_DIR)/$(TEST_DIR)/%.c.o: $(TEST_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)


