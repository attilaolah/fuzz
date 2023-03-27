"""Configure+Make library macro.

Contains a convenience macro that wraps configure_make() from
@rules_foreign_cc.

Also contains most of the common functionality needed by Configure+Make, Make
and CMake macros.
"""

load("@rules_foreign_cc//foreign_cc:configure.bzl", "configure_make")
load("//lib:defs.bzl", "repo_name")
load("//tools/archive_symbols:archive_symbols.bzl", "archive_symbols")

def configure_make_lib(
        name,
        lib_source = None,
        out_static_libs = None,
        ignore_undefined_symbols = True,
        **kwargs):
    """Convenience macro that wraps configure_make().

    Args:
      name: Passed on to configure_make(). Also used for guessing other
        parameters.
      lib_source: Passed on to configure_make(). Guessed from name.
      out_static_libs: Passed on to configure_make(). Guessed from name.
      ignore_undefined_symbols: Whether to ignore undefined symbols. If False,
        any undefined symbols is the archive that are not provided by any of
        the dependencies will cause a build error for the archive symbols
        target.
      **kwargs: Passed no configure_make().
    """
    if lib_source == None:
        lib_source = _lib_source(name)
    if out_static_libs == None:
        out_static_libs = ["lib{}.a".format(name)]

    configure_make(
        name = name,
        lib_name = "{}_lib".format(name),
        lib_source = lib_source,
        out_static_libs = out_static_libs,
        **kwargs
    )

    archive_symbols(
        archive = name,
        deps = kwargs.get("deps", []),
        strict = not ignore_undefined_symbols,
    )

def lib_source(lib_name):
    return "@{}//:all".format(repo_name(lib_name))

_lib_source = lib_source
