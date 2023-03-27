"""Workspace rule for downloading package dependencies."""

load("//:http_archive.bzl", "http_archive")
load("//:versions.bzl", "OS_VERSION")
load("//toolchains:utils.bzl", "patch_files")

VERSION = "12.0.0"
VERSION_MMP = VERSION.split("-")[0]
VERSION_ND = VERSION.replace("-", "")

URL = "https://github.com/llvm/llvm-project/releases/download/llvmorg-{version}/clang+llvm-{version}-x86_64-linux-gnu-" + OS_VERSION + ".tar.xz"
SHA256 = "a9ff205eb0b73ca7c86afc6432eed1c2d49133bd0d49e47b15be59bbf0dd292e"

FLANG_URL = "https://github.com/llvm/llvm-project/releases/download/llvmorg-{version}/flang-{version}.src.tar.xz"
FLANG_SHA256 = "dc9420c9f55c6dde633f0f46fe3f682995069cc5247dfdef225cbdfdca79123a"

BUILD = """
package(default_visibility = ["//visibility:public"])

filegroup(
    name = "all",
    srcs = glob(["**"]),
)

filegroup(
    name = "nm",
    srcs = ["bin/llvm-nm"],
)
"""

def deps():
    http_archive(
        name = "llvm",
        version = VERSION,
        urls = [URL],
        sha256 = SHA256,
        strip_prefix = "clang+llvm-{version}-x86_64-linux-gnu-" + OS_VERSION,
        build_file_content = BUILD,
        patch_cmds = patch_files({
            # Avoid dependency on system libtinfo.so.
            # It breaks the Flang build. Instead, we'll supply a static libtinfo.a archive.
            "lib/cmake/llvm/LLVMExports.cmake": r"s:;/usr/lib/x86_64-linux-gnu/libtinfo.so;:;:g",
        }),
    )
    http_archive(
        name = "flang",
        version = VERSION,
        urls = [FLANG_URL],
        sha256 = FLANG_SHA256,
        strip_prefix = "flang-{version}.src",
        patches = ["//tools/llvm:flang.patch"],
    )
