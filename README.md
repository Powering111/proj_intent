# Introduction

It is a simple game using raylib.


# How to build

This project is based on the template from [Raylib-Quickstart](https://github.com/raylib-extras/raylib-quickstart).

You'll need to first add PATH to w64devkit or equivalent. Adding `(raylib installation directory)\w64devkit\bin` to PATH is sufficient.

To build, run `build-MinGW-W64.bat` and then run `make SHELL=cmd`. Binary will be at `bin` directory.

There are multiple configurations. For example, running `make SHELL=cmd config=release_x64` generates release build.


