programFilename := pong

.PHONY: setup
setup:
# Create build folders
	mkdir -p build
	mkdir -p build/debug
	mkdir -p build/release
	mkdir -p build/dist
# Setup cmake
	cd build/debug && \
	cmake -DBUILD_DEBUG=1 ../.. && \
	cd ../release && \
	cmake -DBUILD_RELEASE=1 ../.. && \
	cd ../dist && \
	cmake -DBUILD_DIST=1 ../..

.PHONY: format
format:
	clang-format src/* -i

.PHONY: clean
clean:
	rm -r build && \
	rm -r bin

.PHONY: build-debug
build-debug:
	cd build/debug && \
	make

.PHONY: build-release
build-release:
	cd build/release && \
	make

.PHONY: build-dist
build-dist:
	cd build/dist && \
	make

.PHONY: run-debug
run-debug:
	cd bin/debug && \
	./${programFilename}

.PHONY: run-release
run-release:
	cd bin/release && \
	./${programFilename}

.PHONY: run-dist
run-dist:
	cd bin/dist && \
	./${programFilename}