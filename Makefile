SHELL = /bin/sh

.SILENT: 

.PHONY: all clean

BUILD_DIR := build
SRC_DIR := src

CC := clang
CFLAGS := -I. -g3 -std=c99 -ferror-limit=1 -MMD -Weverything -Werror \
		  -Wno-poison-system-directories -Wno-unused-function -Wno-padded -Wno-bad-function-cast -Wno-unused-macros -Wno-declaration-after-statement \
		  -Wno-unused-local-typedef \
		  -O3
		  # -DDEBUG -DTAP_DEBUG

OBJCFLAGS := -I.

# dep/sokol/sokol_app.h:63 compile for macOS metal
LD_FLAGS := -framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit 
SAN_FLAGS := -fsanitize=address -fsanitize=undefined 

SRCS := $(wildcard $(SRC_DIR)/*.m) $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(filter %.c, $(SRCS))) $(patsubst %.m, $(BUILD_DIR)/%.o, $(filter %.m, $(SRCS)))
DEPS = $(OBJS:.o=.d)

all: $(BUILD_DIR)/main

clean:
	-test -d $(BUILD_DIR) && rm -r $(BUILD_DIR)

# Build the binary
$(BUILD_DIR)/main: $(OBJS)
	$(CC) $^ -o $@ $(LD_FLAGS) $(SAN_FLAGS)

# Build Objective-C src object files
$(BUILD_DIR)/%.o: %.m $(BUILD_DIR)/$(SRC_DIR)/.dir
	$(CC) -c $< -o $@ $(OBJCFLAGS) $(SAN_FLAGS)

# Build C src object files
$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/$(SRC_DIR)/.dir
	$(CC) -c $< -o $@ $(CFLAGS) $(SAN_FLAGS)

$(BUILD_DIR)/.dir:
	mkdir $(BUILD_DIR)
	touch $(BUILD_DIR)/.dir

$(BUILD_DIR)/$(SRC_DIR)/.dir: $(BUILD_DIR)/.dir
	mkdir $(BUILD_DIR)/$(SRC_DIR)
	touch $(BUILD_DIR)/$(SRC_DIR)/.dir

# Include header dependency targets if they've been generated with -MMD.
-include $(DEPS)
