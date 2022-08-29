# CHIP-8- :snowman:

A CHIP-8 interpreter written in C++20.

## Motivation

Interpreters are fascinating, they let us make a ton of awesome things such as compilers, virtual machines, etc.
For this project, I wanted to have a slightly different approach than other CHIP-8 interpreters implementations.

### Instruction set as an array of functions

I'm not the only one to do that, but instead of a big switch-case statement, I used an array of functions to store the instructions sets.
More precisely, it's an array of pointers to member functions.

### Multithreaded interpreter

Rather than using the SFML main loop, which means that the speed of the CHIP-8 speed depends on the frame rate of the display, I made the CHIP-8 CPU run in a separate thread. This allows to configure the delay between each interrupt.

Thanks to this multithreaded approach, it would be fairly easy to have custom delay based on the instruction that is executed, as suggested by [Chip 8 Instruction Scheduling and Frequency](https://jackson-s.me/2019/07/13/Chip-8-Instruction-Scheduling-and-Frequency.html).

Timers also run in their own thread, which makes things a bit easier as well.

### Using an array of bitsets for the screen

The draw instruction is not the easiest one to implement.
The CHIP-8 language handles only black and white, so the screen is made of bits.

A rather clean way to store bits is to use `std::bitset`.
That's why stored the whole screen in an array of bitsets, where each bitset represents a line on the screen.
It allowed me to draw a sprite on the screen using a single `for` loop.

Moreover, the interpreter has to set register `VF` to `1` if any pixel has been changed from `1` to `0`.
Thanks to bitsets, this is accomplished via the following one-liner: 

```cpp
collision |= (prevScreenLine & ~screenline).any();
```

### Use of some C++20 features

Writing this interpreter was also an opportunity to get used to some C++20 features, such as the ones that are available in the `<bit>` header.
I also make use of some other minor features and the ranges library.

## Dependencies

This project depends on [SFML](https://www.sfml-dev.org/).
Make sure it is installed before you build the executable.
The project uses `CMake`, so you will also have to install it to compile the code.

## Build

### Linux

Using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

### Windows

Configure the project using CMake GUI, or CMake Tools using vscode, for instance.
Then you can build the executable using Visual Studio 2022, or vscode.

### MacOS

Install CMake, SFML, and gcc using brew.
It should work with a recent version of clang, but it'll work with with gcc-12 for sure.
Make sure the compiler that is going to be used is gcc:

```bash
export CXX=/usr/local/bin/g++-12
```

Then build the project using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

The executable takes one argument that indicates the path of a ROM, for instance:

```bash
./chip8pp BRIX
```

Please refer to next section to see where to find ROMs.

## ROMs

There are many ways to find ROMs for the CHIP-8 interpreter, however, not all of the will work.
The reason is that there are different specifications of the CHIP-8, such as SuperChip, CHIP-48, etc.

The best thing to do is to get the ROMs from the [Chip-8 Program Pack](https://github.com/kripod/chip8-roms).

## Keypad

You can change the keypad layout by editing the `KeyPad.hpp` file.
Make sure to re-compile the program after you've edited the file.

## ToDo

- Fix bugs, especially FX instructions.
- Base timing on Cosmac VIP instructions execution time (see [Chip 8 Instruction Scheduling and Frequency](https://jackson-s.me/2019/07/13/Chip-8-Instruction-Scheduling-and-Frequency.html)).
- Configurable keypad.

## Resources

[Mastering CHIP‐8](https://github.com/mattmikolay/chip-8/wiki/Mastering-CHIP%E2%80%908)

[Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

## Licence

This work is released under the MIT licence.
