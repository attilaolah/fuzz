"""Repository rules for downloading all dependencies."""

load(":http_archive.bzl", "http_archive")
load("//toolchains:utils.bzl", "patch_files")

def workspace_dependencies():
    """Set up dependencies of THIS workspace."""
    http_archive(
        name = "rules_foreign_cc",
        version = "0.9.0",
        urls = ["https://github.com/bazelbuild/{name}/archive/{version}.tar.gz"],
        sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
        strip_prefix = "{name}-{version}",
        build_file_content = None,
    )

    http_archive(
        name = "io_bazel_rules_go",
        version = "0.38.1",
        urls = [
            "https://github.com/bazelbuild/rules_go/releases/download/v{version}/rules_go-v{version}.zip",
            "https://mirror.bazel.build/github.com/bazelbuild/rules_go/releases/download/v{version}/rules_go-v{version}.zip",
        ],
        sha256 = "dd926a88a564a9246713a9c00b35315f54cbd46b31a26d5d8fb264c07045f05d",
        build_file_content = None,
    )

    http_archive(
        name = "bazel_gazelle",
        version = "0.27.0",
        urls = [
            "https://github.com/bazelbuild/bazel-gazelle/releases/download/v{version}/bazel-gazelle-v{version}.tar.gz",
            "https://mirror.bazel.build/github.com/bazelbuild/bazel-gazelle/releases/download/v{version}/bazel-gazelle-v{version}.tar.gz",
        ],
        sha256 = "efbbba6ac1a4fd342d5122cbdfdb82aeb2cf2862e35022c752eaddffada7c3f3",
        build_file_content = None,
    )

    http_archive(
        name = "platforms",
        version = "0.0.6",
        urls = [
            "https://github.com/bazelbuild/{name}/releases/download/{version}/{name}-{version}.tar.gz",
            "https://mirror.bazel.build/github.com/bazelbuild/{name}/releases/download/{version}/{name}-{version}.tar.gz",
        ],
        sha256 = "5308fc1d8865406a49427ba24a9ab53087f17f5266a7aabbfc28823f3916e1ca",
        build_file_content = None,
    )

    http_archive(
        name = "bazel-skylib",
        version = "1.4.0",
        urls = [
            "https://github.com/bazelbuild/{name}/releases/download/{version}/{name}-{version}.tar.gz",
            "https://mirror.bazel.build/github.com/bazelbuild/{name}/releases/download/{version}/{name}-{version}.tar.gz",
        ],
        sha256 = "f24ab666394232f834f74d19e2ff142b0af17466ea0c69a3f4c276ee75f6efce",
        build_file_content = None,
    )

    http_archive(
        name = "com_google_protobuf",
        version = "3.19.4",
        urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.19.4.tar.gz"],
        sha256 = "3bd7828aa5af4b13b99c191e8b1e884ebfa9ad371b0ce264605d347f135d2568",
        strip_prefix = "protobuf-{version}",
        build_file_content = None,
    )

    http_archive(
        name = "com_github_bazelbuild_buildtools",
        version = "4.2.2",
        urls = ["https://github.com/bazelbuild/buildtools/archive/refs/tags/{version}.tar.gz"],
        sha256 = "ae34c344514e08c23e90da0e2d6cb700fcd28e80c02e23e4d5715dddcb42f7b3",
        strip_prefix = "buildtools-{version}",
        build_file_content = None,
    )

    http_archive(
        name = "rules_rust",
        version = "0.14.0",
        sha256 = "dd79bd4e2e2adabae738c5e93c36d351cf18071ff2acf6590190acf4138984f6",
        urls = ["https://github.com/bazelbuild/rules_rust/releases/download/{version}/rules_rust-v{version}.tar.gz"],
        build_file_content = None,
    )

    http_archive(
        name = "com_google_googletest",
        version = "1.13.0",
        sha256 = "ad7fdba11ea011c1d925b3289cf4af2c66a352e18d4c7264392fead75e919363",
        urls = ["https://github.com/google/googletest/archive/refs/tags/v{version}.tar.gz"],
        strip_prefix = "googletest-{version}",
        build_file_content = None,
        patch_cmds = patch_files({
            "googletest/include/gtest/gtest-printers.h": "s/__cpp_char8_t/__cpp_lib_char8_t/g",
            "googletest/src/gtest-printers.cc": "s/__cpp_char8_t/__cpp_lib_char8_t/g",
        }),
    )

    http_archive(
        name = "rules_fuzzing",
        version = "0.3.2",
        sha256 = "d9002dd3cd6437017f08593124fdd1b13b3473c7b929ceb0e60d317cb9346118",
        urls = ["https://github.com/bazelbuild/{name}/archive/v{version}.zip"],
        strip_prefix = "{name}-{version}",
        build_file_content = None,
    )
