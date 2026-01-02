SHELL := /bin/bash

.PHONY: $(MAKECMDGOALS)

clean:
	@rm -rf build cmake-build*

.ONESHELL:
build:
	mkdir -p build
	pushd build
	cmake .. -G Ninja
	cmake --build .
	popd

rebuild: clean build

run: build
	@./build/Minecraft

format:
	@find src -type f -regex ".*\.\(h\|c\|hpp\|cpp\)$$" | xargs clang-format -i
