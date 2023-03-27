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
$ docker run -it -v "${{`{PWD}`}}:/build" fuzz
```

## Libraries

| Build Label | Version |
|-------------|---------|
{{.Libraries}}

## Tools

| Build Label | Version |
|-------------|---------|
{{.Tools}}
