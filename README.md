<h1 align="center">fcppm</h1>
<p align="center">
    The fcpp project manager
    <br />
    <img src="https://img.shields.io/badge/C%2B%2B-17-b4befe?style=for-the-badge&labelColor=1e1e2e" />
    <img src="https://img.shields.io/badge/license-MIT-a6e3a1?style=for-the-badge&labelColor=1e1e2e" />
    <img src="https://img.shields.io/badge/version-1.0.0-fab387?style=for-the-badge&labelColor=1e1e2e" />
</p>

A lightweight, modular project manager written in **C++17**\
It provides an opinionated build system and project structure for C++ projects

## Features

- **Build management**
- **Project scaffolding** via `fcppm new`
- **TOML project metadata parsing** (`fcpp.toml`)
- **Self-contained utility functions** (No dependencies beyond brief-cpp and stdlib)

## Installation

### Prerequisites

- **C++17** compiler (`g++` or `clang++`)
- [`brief-cpp`](https://github.com/forgor0x10/brief-cpp) in your include path
- **make**
- **clang-tidy**

### Clone and Build

```bash
git clone https://github.com/forgor0x10/fcppm.git
cd fcppm
make
make install
```

## Usage

```bash
fcppm <command> [options]
```

### Commands

| Command | Description                                     |
| ------- | ----------------------------------------------- |
| help    | Show usage help                                 |
| info    | Show info about the current project             |
| new     | Create a new project (e.g. `fcppm new -n demo`) |
| build   | Build the project (`-s` to disable clang-tidy)  |
| clean   | Remove build and target directories             |

### Example

```bash
$ fcppm new -n hello_world
fcppm 1.0.0
> Success: Created project hello_world

$ cd a
$ fcppm build
fcppm 1.0.0
> Tidying: src/main.cpp
> Creating build directory
> Compiling: src/main.cpp
> Creating target directory
> Linking: target/hello_world
> Success: Built project hello_world
```

### Project structure

```
hello_world
└── src
    └── main.cpp
├── .clang-format
├── .clang-tidy
├── .gitignore
└── fcpp.toml
```

### Project Metadata

Defined in `fcpp.toml`:

```toml
[project]
name = "fcppm"
description = "fcpp project manager"
version = "1.0.0"
```
