MAKEFLAGS += -j$(shell nproc)

BUILD_DIR := out/build
SRC_DIR := src
TEST_DIR := test

# Find all the C files we want to compile
SRCS := $(shell find $(SRC_DIR) -name '*.c')
TESTS := $(shell find $(TEST_DIR) -name '*.c')

# object filepath = BUILD_DIR + source filepath + .o
SRC_OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TESTS:%=$(BUILD_DIR)/%.o)

# test binary = test object filepath - .c.o
TEST_BINS = $(patsubst %.c.o,%,$(TEST_OBJS))
TEST_PHONY = $(addsuffix .test, $(TEST_BINS))


# The -MMD and -MP flags generate Makefiles with .d suffix
CFLAGS := -MMD -MP


build: $(BUILD_DIR)/libctap.a

# Build the test binaries
$(TEST_BINS): %: %.c.o $(BUILD_DIR)/libctap.a
	$(CC) $(CFLAGS) -o $@ $< -L$(BUILD_DIR) -lctap
	./$@

# Build the static library
$(BUILD_DIR)/libctap.a: $(SRC_OBJS)
	ar rcs $(BUILD_DIR)/libctap.a $(SRC_OBJS)

# Build src and test object files
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)


