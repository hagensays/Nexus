#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd -- "$script_dir/.." && pwd)"
# shellcheck source=llvm-mingw.lock
source "$script_dir/llvm-mingw.lock"

"$script_dir/bootstrap-toolchain.sh"

toolchain_dir="$repo_root/.toolchains/$LLVM_MINGW_DIRECTORY"
build_dir="$repo_root/build/windows"

cmake -S "$repo_root" -B "$build_dir" -G Ninja \
    -DCMAKE_SYSTEM_NAME=Windows \
    -DCMAKE_CXX_COMPILER="$toolchain_dir/bin/x86_64-w64-mingw32-clang++" \
    -DCMAKE_RC_COMPILER="$toolchain_dir/bin/x86_64-w64-mingw32-windres" \
    -DCMAKE_BUILD_TYPE=Release \
    -DNEXUS_BUILD_TESTS=OFF

cmake --build "$build_dir"

executable="$build_dir/bin/Nexus.exe"
if [[ ! -f "$executable" ]]; then
    echo "Expected executable was not produced: $executable" >&2
    exit 1
fi

echo "Built Windows executable: $executable"
sha256sum "$executable"
