## Powderpunk MMO

Powderpunk is a pretentious project for a MMO.

## Requirements
This project is intended to use a C++20 compiler and it requires the following installed programs:
- CMake (used 3.17)
- Boost Source Libraries (used 1.72)

It also requires the [gsl-lite](https://github.com/gsl-lite/gsl-lite/) and [telnet++](https://github.com/KazDragon/telnetpp) repositories, added as git submodules.

## How to clone it:
As the repository has submodules, it is required you clone with the following command:

```
git clone --recurse-submodules --jobs 2  https://github.com/mrtheduts/powderpunk_mmo.git
```
> The `--jobs 2` is used to fetch up to 2 submodules at a time in parallel

This _should_ download all submodules. If it doesn't, just run:
```
git submodule init
git submodule update
```

## How to build it:
It is recommended to generate the build files in a separated folder, as `./build/`:
1) `cmake . -DCMAKE_BUILD_TYPE=[Release|Debug] -Bbuild`
2) `cmake --build build/`

When you build the project, the `compile_commands.json` file is created, to provide a LSP build information about the project.
