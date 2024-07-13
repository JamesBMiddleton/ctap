# https://makefiletutorial.com/ - adapt the bottom example

SHELL = /bin/bash
CC = usr/bin/gcc
CFLAGS = -g

BUILD_DIR := ./build
# SRC_DIR :=


.PHONY: all
all: format build sanitize test 

.PHONY: build
build:
	echo "Hello world"

.PHONY: format
format:
	echo "Hello world"

.PHONY: sanitize
sanitize:
	echo "Hello world"

.PHONY: test
test:
	echo "Hello world"

.PHONY: clean
clean:
	echo "Hello world"
	# rm out/build
