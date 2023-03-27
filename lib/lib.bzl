load("@bazel_skylib//:bzl_library.bzl", "bzl_library")

def lib_package(runtime_for = ()):
    """Common package contents for most packages under //lib."""
    bzl_library(
        name = "package",
        srcs = ["package.bzl"],
        deps = [
            "//lib:defs",
            "//lib:http_archive",
        ],
    )

    for label in runtime_for:
        name = "runtime"
        if len(runtime_for) > 1:
            name = "_".join((label, name))
        native.filegroup(
            name = name,
            srcs = [":{}".format(label)],
            output_group = "gen_dir",
        )
