
# Asemu

A simple emulator in C++ built around a custom designed 24 bits cpu instruction set.

Made with the purpose of experimenting with C++, git, IDEs, build systems and coding assembly 24 bits style.

Currently you can punch hex code to boostrap your dream virtual computer or game console.

Demo is just flickering pixels by pressing arrows until screen is filled with black.

No documentation present to help.

## Features

* custom CPU instruction set with 24-bit words (interpreted at 16 MHz)
* memory (16MiB)
* keyboard input (QWERTY)
* video and audio output (320x240 RGB888 image, 12bit 48kHz 2 channel audio)

## Compiling

### Dependencies

* SDL2

### Building with Makefile

```bash
make
```

### Building with CMake

```bash
cmake -S . -B build
cd build
make
```

### Building with Meson

```bash
meson build
cd build
ninja
```
