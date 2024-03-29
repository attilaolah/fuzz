"""CMake library macro.

Contains a convenience macro that wraps cmake() from
@rules_foreign_cc.
"""

load("@rules_foreign_cc//foreign_cc:cmake.bzl", "cmake")
load("//lib:defs.bzl", "include_dir", "library_path")
load("//toolchains/make:configure.bzl", _lib_source = "lib_source")

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

def cache_entries(*originals, upcase = True, prefix_all = "", deps = None, **kwargs):
    """Convenience macro for constructing the cache_entries dict."""
    result = {}
    for original in originals + (kwargs,):
        result.update(original)

    remap = result.pop("remap", {})

    for dep, spec in (deps or {}).items():
        prefix = spec.get("prefix", "")
        suffix = spec.get("suffix", "")
        result["{}{}{}_library".format(prefix, dep, suffix)] = spec.get("library", library_path(dep))
        result["{}{}{}_include_dir".format(prefix, dep, suffix)] = spec.get("include_dir", include_dir(dep))

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
