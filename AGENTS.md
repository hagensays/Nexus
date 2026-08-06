# Nexus agent rules

## Product direction

Nexus is a lean, modular Windows productivity application written in C++20.

Technical foundation:

- minimal CMake
- Win32 API
- Windows x64
- Linux-to-Windows cross-compilation with pinned LLVM-MinGW
- no Visual Studio requirement
- no external runtime or UI framework unless explicitly justified
- one portable `Nexus.exe` as the initial deliverable

The product roadmap is maintained separately in `roadmap.md`. Do not duplicate the roadmap here.

## Repository structure

```text
Nexus/
├── CMakeLists.txt
├── README.md
├── AGENTS.md
├── roadmap.md
├── .gitignore
├── assets/
├── tools/
├── src/
│   ├── Nexus.App/
│   ├── Nexus.Core/
│   ├── Nexus.UI/
│   ├── Nexus.Platform/
│   └── Nexus.Modules/
└── tests/
```

Only add folders and abstractions when current code needs them.

## Responsibilities

### Nexus.App

Owns startup, application lifetime, composition, navigation wiring, and module registration. Keep business logic out of this layer.

### Nexus.Core

Owns UI- and platform-independent C++ logic, contracts, shared data types, settings models, commands, recent-item logic, and error handling.

`Nexus.Core` must not include Win32 headers or depend on `Nexus.UI`, `Nexus.Platform`, or a feature module.

### Nexus.UI

Owns the shared user interface, window layout, navigation controls, dialogs, typography, spacing, and themes. Feature business logic does not belong here.

### Nexus.Platform

Owns Windows-specific integration such as application bootstrap, Shell operations, native dialogs, clipboard access, process launching, system paths, printing, and notifications.

### Nexus.Modules

Owns product features. Each major feature belongs in its own folder. Modules may depend on Core and shared UI contracts but must not directly depend on another module.

## Dependency direction

Allowed:

```text
Nexus.App      -> Core, UI, Platform, Modules
Nexus.Modules  -> Core, UI
Nexus.UI       -> simple Core types when necessary
Nexus.Platform -> Core interfaces
```

Not allowed:

```text
Nexus.Core -> UI or Platform
Module A   -> Module B
```

## GitHub policy

GitHub is a source-code store and commit history, not the build environment.

- Do not add GitHub Actions unless the user explicitly changes this policy.
- Do not rely on GitHub-hosted builds or tests.
- Work directly on `master` unless the user explicitly requests a branch.
- Keep commits focused on the requested change.
- Do not perform unrelated cleanup.

## Toolchain

The Windows cross-compiler is pinned in `tools/llvm-mingw.lock`.

- Use `tools/bootstrap-toolchain.sh` to obtain and verify the exact toolchain.
- Use `tools/build-windows.sh` for the Windows x64 build.
- Use `tools/test-linux.sh` for Linux-hosted Core and module tests.
- Do not silently replace the pinned archive, URL, or checksum.
- When changing the toolchain, verify its license, checksum, compiler version, Windows build, and imported runtime DLLs.
- Downloaded compiler files live under `.toolchains/` and are not committed to Git history.

## Build and verification

Use the available development environment for builds and tests.

Before finalizing a code change when tooling is available:

1. Run `./tools/test-linux.sh`.
2. Run `./tools/build-windows.sh`.
3. Confirm that `build/windows/bin/Nexus.exe` exists and is a Windows x64 GUI executable.
4. Inspect imported DLLs when build-system or runtime-linking behavior changes.
5. Run a startup smoke test through Wine or Windows when a reliable runtime is available.

If a required compiler or runtime is unavailable, report the exact missing verification instead of claiming success.

## Development rules

- Prefer the C++ standard library and Win32 API.
- Add external dependencies only for a concrete, documented benefit.
- Keep the build as one executable until separation into libraries provides real value.
- Add tests for Core and non-UI module behavior.
- Keep Windows-specific types out of Core interfaces.
- Avoid speculative plugin systems, service locators, or deep abstraction layers.
- Preserve a quiet, compact, functional UI.
- Never publish or describe an unverified build as working.
