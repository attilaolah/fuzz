"""Workspace rule for downloading package dependencies."""

load("//lib:http_archive.bzl", "http_archive")
load("//toolchains:utils.bzl", "patch_files")

NAME = "protobuf_mutator"
VERSION = "1.1"

URL = "https://github.com/google/libprotobuf-mutator/archive/refs/tags/v{version}.tar.gz"

SHA256 = "fd299fd72c5cf664259d9bd43a72cb74dc6a8b9604d107fe2d2e90885aeb7c16"

def deps():
    http_archive(
        name = NAME,
        version = VERSION,
        urls = [URL],
        sha256 = SHA256,
        strip_prefix = "libprotobuf-mutator-{version}",
        #add_subdirectory(examples EXCLUDE_FROM_ALL)
        patch_cmds = patch_files({
            # We don't want to build the examples.
            "CMakeLists.txt": r"s/add_subdirectory\(examples.*//g",
            # Fix a quoted import to make it easier to resolve.
            "src/libfuzzer/libfuzzer_macro.h": ":".join([
                "s",
                '#include "port/protobuf.h"',
                '#include "../../port/protobuf.h":',
            ]),
        }),
    )
