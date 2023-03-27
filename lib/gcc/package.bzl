"""Workspace rule for downloading package dependencies."""

load("//lib:http_archive.bzl", "http_archive")

NAME = "gcc"
VERSION = "12.2.0"

URLS = [
    "https://ftp.gnu.org/gnu/{name}/{name}-{version}/{name}-{version}.tar.xz",
    "https://mirror.kumi.systems/gnu/{name}/{name}-{version}/{name}-{version}.tar.xz",
]

SHA256 = "e549cf9cf3594a00e27b6589d4322d70e0720cdd213f39beb4181e06926230ff"

def deps():
    http_archive(
        name = NAME,
        version = VERSION,
        urls = URLS,
        sha256 = SHA256,
        strip_prefix = "{name}-{version}",
        patch_cmds = [
            # Contains a file with a funky filename.
            "rm -rf gcc/testsuite/go.test/test/fixedbugs/issue27836.dir",
        ],
    )
