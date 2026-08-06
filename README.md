# Nexus

Nexus is a lean, modular Windows productivity application written in C++20.

## Technical foundation

- C++20
- minimal CMake build
- Win32 API
- Windows x64 target
- Linux-to-Windows cross-compilation with pinned LLVM-MinGW
- no external runtime or UI framework
- GitHub used only for source storage and commit history

## Structure

```text
src/
├── Nexus.App       application entry point and composition
├── Nexus.Core      UI- and platform-independent logic
├── Nexus.UI        shared Windows user interface
├── Nexus.Platform  Windows-specific integration
└── Nexus.Modules   independent product features
```

The current bootstrap includes a small Home module, a Settings module, a Win32 main window, and Linux-buildable Core tests.

## Test Core and modules on Linux

```bash
./tools/test-linux.sh
```

## Build Nexus.exe on Linux

The repository pins LLVM-MinGW `20260616` and verifies its SHA-256 checksum before use.

Normal setup and build:

```bash
./tools/build-windows.sh
```

The bootstrap script downloads the exact pinned archive when it is not already present. A pre-downloaded archive can be supplied explicitly:

```bash
NEXUS_TOOLCHAIN_ARCHIVE=/path/to/llvm-mingw-20260616-ucrt-ubuntu-22.04-x86_64.tar.xz \
  ./tools/build-windows.sh
```

Expected output:

```text
build/windows/bin/Nexus.exe
```

Toolchain metadata is stored in `tools/llvm-mingw.lock`. The upstream archive includes its license as `LICENSE.TXT`.

See `AGENTS.md` for development rules and `roadmap.md` for milestones.
