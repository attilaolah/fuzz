"""Workspace rule for downloading package dependencies."""

load("//lib:http_archive.bzl", "http_archive")

NAME = "gif"
VERSION = "5.2.1"

URL = "https://downloads.sourceforge.net/project/{name}lib/{name}lib-{version}.tar.gz"

SHA256 = "31da5562f44c5f15d63340a09a4fd62b48c45620cd302f77a6d9acf0077879bd"

def deps():
    http_archive(
        name = NAME,
        version = VERSION,
        urls = [URL],
        sha256 = SHA256,
        strip_prefix = "{name}lib-{version}",
    )
