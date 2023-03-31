"""Make library macro.

Contains a convenience macro that wraps make() from @rules_foreign_cc.
"""

load("@rules_foreign_cc//foreign_cc:make.bzl", "make")
load(":configure.bzl", _lib_source = "lib_source")

def make_lib(
        name,
        lib_source = None,
        out_static_libs = None,
        ignore_undefined_symbols = True,
        **kwargs):
    """Convenience macro that wraps make().

    Args:
      name: Passed on to make(). Also used for guessing other parameters.
      lib_source: Passed on to make(). Guessed from name.
      out_static_libs: Passed on to make(). Guessed from name.
      **kwargs: Passed no make().
    """
    if lib_source == None:
        lib_source = _lib_source(name)
    if out_static_libs == None:
        out_static_libs = ["lib{}.a".format(name)]
    kwargs.setdefault("args", []).extend(["--environment-overrides"])

    make(
        name = name,
        lib_name = "{}_lib".format(name),
        lib_source = lib_source,
        out_static_libs = out_static_libs,
        **kwargs
    )
