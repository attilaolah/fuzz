"""CMake library macro.

Contains a convenience macro that wraps cmake() from
@rules_foreign_cc.
"""

load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")
load("//lib:cache_entries.bzl", "include_dir_key", "library_key")
load("//lib:defs.bzl", "dep_spec")
load("//toolchains/make:configure.bzl", _lib_source = "lib_source")
load("//tools/archive_symbols:archive_symbols.bzl", "archive_symbols")

def cmake_lib(
        name,
        lib_source = None,
        out_static_libs = None,
        cache_entries = None,
        ignore_undefined_symbols = True,
        **kwargs):
    """Convenience macro that wraps cmake().

    Args:
      name: Passed on to cmake(). Also used for guessing other parameters.
      lib_source: Passed on to cmake(). Guessed from name.
      out_static_libs: Passed on to cmake(). Guessed from name.
      cache_entries: Convert True/False to "ON"/"OFF", then passed on to
        cmake().
      ignore_undefined_symbols: Whether to ignore undefined symbols. If False,
        any undefined symbols is the archive that are not provided by any of
        the dependencies will cause a build error for the archive symbols
        target.
      **kwargs: Passed on to cmake().
    """
    if lib_source == None:
        lib_source = _lib_source(name)
    if out_static_libs == None:
        out_static_libs = ["lib{}.a".format(name)]

    cmake(
        name = name,
        cache_entries = _cache_entries(cache_entries or {}),
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

def cache_entries(*originals, upcase = True, prefix_all = "", deps = None, **kwargs):
    """Convenience macro for constructing the cache_entries dict."""
    result = {}
    for original in originals + (kwargs,):
        result.update(original)

    remap = result.pop("remap", {})

    for dep, spec in (deps or {}).items():
        spec = spec or dep_spec(dep)
        if "include_dir" in spec:
            result[include_dir_key(dep)] = spec["include_dir"]
        if "library" in spec:
            result[library_key(dep)] = spec["library"]

    for new, old in remap.items():
        result[new] = result.pop(old)

    if upcase:
        for key in list(result):
            result[key.upper()] = result.pop(key)

    return {prefix_all + key: val for key, val in result.items()}

def _cache_entries(cache_entries):
    cache_entries = {key: val for key, val in cache_entries.items()}
    for key, val in cache_entries.items():
        if val in (True, False):
            cache_entries[key] = "ON" if val else "OFF"
    return cache_entries
