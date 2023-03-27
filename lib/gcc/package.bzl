"""Workspace rule for downloading package dependencies."""

load("//lib:http_archive.bzl", "http_archive")

NAME = "gcc"
VERSION = "10.4.0"

URLS = [
    "https://ftp.gnu.org/gnu/{name}/{name}-{version}/{name}-{version}.tar.xz",
    "https://mirror.kumi.systems/gnu/{name}/{name}-{version}/{name}-{version}.tar.xz",
]

SHA256 = "c9297d5bcd7cb43f3dfc2fed5389e948c9312fd962ef6a4ce455cff963ebe4f1"

def deps():
    http_archive(
        name = NAME,
        version = VERSION,
        urls = URLS,
        sha256 = SHA256,
        strip_prefix = "{name}-{version}",
    )
