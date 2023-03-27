"""Workspace rule for downloading subpackage dependencies."""

load("//lib/gcc:package.bzl", _gcc_deps = "deps")
load("//lib/gif:package.bzl", _gif_deps = "deps")
load("//lib/iconv:package.bzl", _iconv_deps = "deps")
load("//lib/musl:package.bzl", _musl_deps = "deps")
load("//lib/ncurses:package.bzl", _ncurses_deps = "deps")
load("//lib/xml:package.bzl", _xml_deps = "deps")
load("//lib/z:package.bzl", _z_deps = "deps")

def deps():
    """Download all library sources."""
    _gcc_deps()
    _gif_deps()
    _iconv_deps()
    _musl_deps()
    _ncurses_deps()
    _xml_deps()
    _z_deps()
