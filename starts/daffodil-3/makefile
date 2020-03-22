# This makefile just wraps CMAKE.
# Relevent targets are 'run' and 'debug'.

run: build-release/main
	build-release/main

debug: build-debug/main
	gdb buildebug/main

build-%/main: build-%/CMakeCache.txt always-process
	cd build-$* && make

build-%/CMakeCache.txt: CMakeLists.txt makefile
	mkdir -p build-$*
	cd build-$* && cmake .. -DCMAKE_BUILD_TYPE=$*

always-process:
