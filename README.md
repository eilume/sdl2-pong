# SDL2 Pong

> A basic pong clone created using SDL2.

<p align="center">
    <img src="docs/demo.gif">
</p>

[Play in a web browser!](https://eilume.itch.io/sdl-pong)

This was a (mostly) weekend exercise to learn C++ and SDL2, specifically focused on memory management via the usage of `std::shared_ptr` and general code structure/architecture.

## Controls

- **W/S** - Player One (Left)
- **Arrow Up/Down** - Player Two (Right)

## Asset Licenses

- [FFFFORWA Font](https://www.1001fonts.com/fff-forward-font.html) - [See license here](/assets/fonts/FFFFORWA_LICENSE.txt)
- [Kenney Impact Sounds](https://kenney.nl/assets/impact-sounds) - [See license here](/assets/audio/sfx/License.txt)

## Libraries

- [SDL](https://github.com/libsdl-org/SDL) - Cross-platform library for windowing, graphics, input, audio 
- [SDL_image](https://github.com/libsdl-org/SDL_image) - Image loader library for SDL
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf) - Font loader + renderer library for SDL
- [SDL_mixer](https://github.com/libsdl-org/SDL_mixer) - Simple multi-channel audio mixer for SDL

## Compiling

### Requirements

- C++ Compiler (Tested with LLVM's [Clang](https://clang.llvm.org/) v13.1.6 using `-std=c++17`)
- [CMake](https://cmake.org/) - Cross-platform build environment setup

#### Web Only

- [Emscripten](https://emscripten.org/) - C/C++ to WASM (WebAssembly) Compiler (Tested with v3.1.13)

### Building

*Note*: Only a [Makefile](/Makefile) is provided, so Windows users will have to convert some commands from the makefile to Win32 alternatives!

1. `git clone --recursive https://github.com/eilume/sdl-pong`
2. `cd sdl-pong`
3. `make setup-[debug|release|dist|all]` **Note:** Not needed for web
4. `make build-[debug|release|dist|web]`
5. `make run-[debug|release|dist]` **Note:** No option for web builds, it'll likely require to be hosted from a server