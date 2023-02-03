SOURCE_DIRECTORY = $(shell pwd)
BUILD_DIRECTORY = /tmp/scalr/build

configure:
	cmake -S $(SOURCE_DIRECTORY) -B $(BUILD_DIRECTORY)

build:
	cmake --build $(BUILD_DIRECTORY)

test: configure build
	cmake --build $(BUILD_DIRECTORY) --target test

install: configure
	cmake --build $(BUILD_DIRECTORY) --target install
