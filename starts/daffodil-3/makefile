# This makefile just wraps CMAKE.
# Relevent targets are 'run' and 'debug'.

run: build-release/main
	build-release/main

debug: build-debug/main
	gdb build-debug/main

build-%/main: build-%/CMakeCache.txt .ALWAYS-PROCESS
	cd build-$* && make

build-%/CMakeCache.txt: CMakeLists.txt makefile
	mkdir -p build-$*
	cd build-$* && cmake .. -DCMAKE_BUILD_TYPE=$*

.PRECIOUS: build-debug/CMakeCache.txt build-release/CMakeCache.txt
.ALWAYS-PROCESS:
