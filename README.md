<p align="left">
  <img src="assets/logo.png" width="60" alt="Scribble" style="vertical-align: middle;">
  <span style="font-size: 2em; font-weight: bold; vertical-align: middle; margin-left: 12px;">Scribble</span>
</p>

Scribble is an interpreted scripting language written in C++. It is heavily in development with no clear goal currently, it's an ongoing experiment in language design and implementation.

## Dependencies

- A C++ compiler (GCC or Clang)
- CMake
- Make or Ninja

That's it. No external libraries, no package manager, no runtime to install.

> Note: Currently only support for Linux exists, with a plan to make it supported on Windows, however it should work fine on windows  if you compile it with CMake, but there is no promises.

## Building

```bash
mkdir -p build
cd build
cmake ..
make
```

Or with Ninja:

```bash
mkdir -p build
cd build
cmake -G Ninja
ninja
```

## Running

```bash
./build/scribble your_file.scribble
```

## Language Docs

- [Variables & Constants](docs/variables.md)
- [Arithmetic](docs/arithmetic.md)
- [Booleans & Comparison](docs/booleans.md)
- [Control Flow](docs/control_flow.md)
- [Functions](docs/functions.md)
- [Print](docs/print.md)

## Examples

See the [examples](examples/) folder for runnable `.scribble` files covering every feature of the language.

## License

MIT