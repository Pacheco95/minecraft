SHELL := /bin/bash

.PHONY: $(MAKECMDGOALS)

clean:
	@rm -rf cmake-build*

.ONESHELL:
build:
	cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -S . -B ./cmake-build-debug

run:
	@./build/Minecraft

format:
	@find src -type f -regex ".*\.\(h\|c\|hpp\|cpp\)$$" | xargs clang-format -i
