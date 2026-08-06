# Nexus

Nexus is a lean, modular Windows productivity application written in C++20.

## Technical foundation

- C++20
- minimal CMake build
- Win32 API
- Windows x64 target
- Linux-to-Windows cross-compilation with MinGW-w64
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

## Build Core tests on Linux

```bash
cmake -S . -B build/linux -G Ninja
cmake --build build/linux
ctest --test-dir build/linux --output-on-failure
```

## Cross-compile Nexus.exe on Linux

Requires MinGW-w64:

```bash
cmake -S . -B build/windows -G Ninja \
  -DCMAKE_SYSTEM_NAME=Windows \
  -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ \
  -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres

cmake --build build/windows
```

Expected output:

```text
build/windows/bin/Nexus.exe
```

See `AGENTS.md` for development rules and `roadmap.md` for milestones.
