![A stampede of Sandworms - by kekaik](https://cg3.cgsociety.org/uploads/images/original/kekaik-stampede-1-2155d580-p9yu.jpeg)
## Powderpunk MMO

Powderpunk is a pretentious project for a MMO.

## Requirements
This project is intended to use a C++20 compiler and it requires the following installed programs:
- CMake (used 3.17)
- Boost Source Libraries (used 1.72)

It also requires the [gsl-lite](https://github.com/gsl-lite/gsl-lite/) and [telnet++](https://github.com/KazDragon/telnetpp) repositories, added as git submodules.

## How to clone it:
As the repository has submodules, it is required you clone this project with the following command:

```shell
$ git clone --recurse-submodules --jobs 2  https://github.com/mrtheduts/powderpunk_mmo.git
```
> The `--jobs 2` is used to fetch up to 2 submodules at a time in parallel

This _should_ download all submodules. If it doesn't, just run:
```shell
$ git submodule init
$ git submodule update
```

## How to build it:
It is recommended to generate the build files in a separated folder, as `./build/`:
```shell
$ cmake . -DCMAKE_BUILD_TYPE=[Release|Debug] -Bbuild
$ cmake --build build/
```

When you build the project, the `./build/compile_commands.json` file is created, to provide a LSP build information about the project.
