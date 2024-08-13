## Powderpunk MMO

> Status: Archived

Powderpunk is a pretentious project for a MMO.

## Requirements
This project is intended to use a C++20 compiler and it requires the following installed program:
- [CMake](https://cmake.org) (used 3.17)

It also requires the following submodules:
- [Boost](https://github.com/boostorg/boost) (boost-1.75.0)
- [gsl-lite](https://github.com/gsl-lite/gsl-lite) (v0.34.0)
- [Telnet++](https://github.com/KazDragon/telnetpp) (v2.1.2)

## How to clone it:
As the repository has submodules, it is required you clone this project with the following command:

```shell
$ git clone --recurse-submodules --jobs 2  https://github.com/mrtheduts/powderpunk_mmo.git
```
> The `--jobs 2` is used to fetch up to 2 submodules at a time in parallel

This _should_ download all submodules. If it doesn't, just run:
```shell
$ git submodule update --init --recursive
```

## How to build it:
As Boost isn't compatible to be built with CMake, it is required to do the following in the `server/external/boost` folder:
```shell
$ ./bootstrap.sh --prefix=../boost_install
$ ./b2 install
```

Next, to build the project with CMake, it is recommended to generate its build files in a separated folder, as `server/build/`:
```shell
$ cmake . -DCMAKE_BUILD_TYPE=[Release|Debug] -Bbuild
$ cmake --build build/
```

When you build the project, the `server/build/compile_commands.json` file is created, to provide a LSP build information about the project.
