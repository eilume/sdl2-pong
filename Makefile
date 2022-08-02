programFilename := pong

.PHONY: setup-all
setup-all:
# Create build folders
	mkdir -p build
	mkdir -p build/debug
	mkdir -p build/release
	mkdir -p build/dist
# Setup cmake
	cd build/debug && \
	cmake -DBUILD_DEBUG=1 -S ../.. -B . && \
	cd ../release && \
	cmake -DBUILD_RELEASE=1 -S ../.. -B . && \
	cd ../dist && \
	cmake -DBUILD_DIST=1 -S ../.. -B .

.PHONY: setup-debug
setup-debug:
	mkdir -p build
	mkdir -p build/debug
	cd build/debug && \
	cmake -DBUILD_DEBUG=1 -S ../.. -B . 

.PHONY: setup-release
setup-release:
	mkdir -p build
	mkdir -p build/release
	cd build/release && \
	cmake -DBUILD_RELEASE=1 -S ../.. -B . 

.PHONY: setup-dist
setup-dist:
	mkdir -p build
	mkdir -p build/dist
	cd build/dist && \
	cmake -DBUILD_DIST=1 -S ../.. -B . 

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

.PHONY: build-web
build-web:
	mkdir -p bin
	mkdir -p bin/web
	emcc ./src/*.cpp -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS='["png"]' -s SDL2_MIXER_FORMATS='["ogg"]' -O3 -o "bin/web/index.html" --preload-file ./assets/ --shell-file src/shell.html

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