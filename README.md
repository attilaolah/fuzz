# Fuzz!

<!-- DO NOT EDIT README.md!

This file was auto-generated based on the template file
`tools/write_me/write_me.tpl`. Update the template file and then re-generate
the `README.md` file by running:

$ bazel run //tools/write_me
-->

Collection of fuzz targets for various open-source libraries.

To get (somewhat) reproducible results, run the builds in a Docker container.
To get a shell within the container, run build the image and run it:

```sh
$ docker build -t fuzz docker
$ docker run -it -v "${PWD}:/build" fuzz
```

## Libraries

| Build Label | Version |
|-------------|---------|
[`//lib/gcc`](https://github.com/attilaolah/fuzz/blob/main/lib/gcc/BUILD.bazel) | 10.4.0 [🔗](https://ftp.gnu.org/gnu/gcc/gcc-10.4.0/gcc-10.4.0.tar.xz) [🔗](https://mirror.kumi.systems/gnu/gcc/gcc-10.4.0/gcc-10.4.0.tar.xz)
[`//lib/gif`](https://github.com/attilaolah/fuzz/blob/main/lib/gif/BUILD.bazel) | 5.2.1 [🔗](https://downloads.sourceforge.net/project/giflib/giflib-5.2.1.tar.gz)
[`//lib/iconv`](https://github.com/attilaolah/fuzz/blob/main/lib/iconv/BUILD.bazel) | 1.16 [🔗](https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.16.tar.gz)
[`//lib/musl`](https://github.com/attilaolah/fuzz/blob/main/lib/musl/BUILD.bazel) | 1.2.3 [🔗](https://musl.libc.org/releases/musl-1.2.3.tar.gz)
[`//lib/ncurses`](https://github.com/attilaolah/fuzz/blob/main/lib/ncurses/BUILD.bazel) | 6.2 [🔗](https://ftp.gnu.org/pub/gnu/ncurses/ncurses-6.2.tar.gz)
[`//lib/xml`](https://github.com/attilaolah/fuzz/blob/main/lib/xml/BUILD.bazel) | 2.9.10 [🔗](http://xmlsoft.org/sources/libxml2-2.9.10.tar.gz)
[`//lib/z`](https://github.com/attilaolah/fuzz/blob/main/lib/z/BUILD.bazel) | 1.2.11 [🔗](https://downloads.sourceforge.net/libpng/zlib-1.2.11.tar.gz)


## Tools

| Build Label | Version |
|-------------|---------|
[`//tools/llvm`](https://github.com/attilaolah/fuzz/blob/main/tools/llvm/BUILD.bazel) | 12.0.0 [🔗](https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.0/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04.tar.xz)

