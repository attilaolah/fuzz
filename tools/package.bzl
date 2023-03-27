"""Workspace rule for downloading subpackage dependencies."""

load("//tools/llvm:package.bzl", _llvm_deps = "deps")

def deps():
    """Download all tools sources."""
    _llvm_deps()
