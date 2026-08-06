#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd -- "$script_dir/.." && pwd)"
# shellcheck source=llvm-mingw.lock
source "$script_dir/llvm-mingw.lock"

toolchains_dir="$repo_root/.toolchains"
toolchain_dir="$toolchains_dir/$LLVM_MINGW_DIRECTORY"
compiler="$toolchain_dir/bin/x86_64-w64-mingw32-clang++"

if [[ -x "$compiler" ]]; then
    echo "LLVM-MinGW already available: $toolchain_dir"
    "$compiler" --version | head -n 1
    exit 0
fi

mkdir -p "$toolchains_dir/downloads"

if [[ -n "${NEXUS_TOOLCHAIN_ARCHIVE:-}" ]]; then
    archive="$NEXUS_TOOLCHAIN_ARCHIVE"
elif [[ -f "$script_dir/cache/$LLVM_MINGW_ARCHIVE" ]]; then
    archive="$script_dir/cache/$LLVM_MINGW_ARCHIVE"
else
    archive="$toolchains_dir/downloads/$LLVM_MINGW_ARCHIVE"
    if [[ ! -f "$archive" ]]; then
        echo "Downloading $LLVM_MINGW_URL"
        if command -v curl >/dev/null 2>&1; then
            curl --fail --location --retry 3 --output "$archive" "$LLVM_MINGW_URL"
        elif command -v wget >/dev/null 2>&1; then
            wget --output-document="$archive" "$LLVM_MINGW_URL"
        else
            echo "Neither curl nor wget is available." >&2
            exit 1
        fi
    fi
fi

if [[ ! -f "$archive" ]]; then
    echo "Toolchain archive not found: $archive" >&2
    exit 1
fi

actual_sha256="$(sha256sum "$archive" | awk '{print $1}')"
if [[ "$actual_sha256" != "$LLVM_MINGW_SHA256" ]]; then
    echo "Toolchain checksum mismatch." >&2
    echo "Expected: $LLVM_MINGW_SHA256" >&2
    echo "Actual:   $actual_sha256" >&2
    exit 1
fi

rm -rf "$toolchain_dir"
tar -xJf "$archive" -C "$toolchains_dir"

if [[ ! -x "$compiler" ]]; then
    echo "Compiler missing after extraction: $compiler" >&2
    exit 1
fi

echo "LLVM-MinGW installed: $toolchain_dir"
"$compiler" --version | head -n 1
