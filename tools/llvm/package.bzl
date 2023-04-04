"""Workspace rule for downloading package dependencies."""

load("//:http_archive.bzl", "http_archive")
load("//toolchains:utils.bzl", "patch_files")

VERSION = "16.0.0"
VERSION_MMP = VERSION.split("-")[0]
VERSION_ND = VERSION.replace("-", "")

# Clang is only pre-built for Ubuntu 18.04 (for now).
# It runs, however, in Ubuntu 22.04, as long as libtinfo5 is installed.
OS_VERSION = "ubuntu-18.04"

URL = "https://github.com/llvm/llvm-project/releases/download/llvmorg-{version}/clang+llvm-{version}-x86_64-linux-gnu-" + OS_VERSION + ".tar.xz"
SHA256 = "2b8a69798e8dddeb57a186ecac217a35ea45607cb2b3cf30014431cff4340ad1"

FLANG_URL = "https://github.com/llvm/llvm-project/releases/download/llvmorg-{version}/flang-{version}.src.tar.xz"
FLANG_SHA256 = "2734b4bb13080b874b0b83c203f611e805ed56a284ce080cf8bcdbfa2a064ff2"

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
        strip_prefix = "clang+{name}-{version}-x86_64-linux-gnu-" + OS_VERSION,
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
        strip_prefix = "{name}-{version}.src",
    )
