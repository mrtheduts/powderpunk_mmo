![A stampede of Sandworms - by kekaik](https://cg3.cgsociety.org/uploads/images/original/kekaik-stampede-1-2155d580-p9yu.jpeg)
## Powderpunk MMO

Powderpunk is a pretentious project for a MMO.

## Requirements
This project is intended to use a C++20 compiler and it requires the following installed program:
- [CMake](https://cmake.org) (used 3.17)

It also requires the following submodules:
- [Boost](https://github.com/boostorg/boost) (boost-1.71.0)
- [gsl-lite](https://github.com/gsl-lite/gsl-lite) (v0.37.0)
- [Telnet++](https://github.com/KazDragon/telnetpp) (master)

## How to clone it:
As the repository has submodules, it is required you clone this project with the following command:

```shell
$ git clone --recurse-submodules --jobs 2  https://github.com/mrtheduts/powderpunk_mmo.git
```
> The `--jobs 2` is used to fetch up to 2 submodules at a time in parallel

This _should_ download all submodules. If it doesn't, just run:
```shell
$ git submodule init
$ git submodule update --recursive
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
