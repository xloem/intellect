# This makefile just wraps CMAKE.
# Relevent targets are 'run' and 'debug'.

run: build/main
	build/main

debug: buildebug/main
	gdb buildebug/main

buil%/main: buil%/CMakeCache.txt always-process
	cd buil$* && make

buil%/CMakeCache.txt: CMakeLists.txt makefile
	mkdir -p buil$*
	cd buil$* && cmake .. -DCMAKE_BUILD_TYPE=$*

always-process:
