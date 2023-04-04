workspace(name = "fuzz")

## Workspace dependencies & toolchains.

load("//:versions.bzl", "GO_VERSION", "RUST_EDITION")
load("//:workspace.bzl", "workspace_dependencies")

workspace_dependencies()

register_toolchains("//toolchains/cc:all")

## Go dependencies & toolchain.

load("@io_bazel_rules_go//go:deps.bzl", "go_register_toolchains", "go_rules_dependencies")
load("//:package.bzl", "deps", "go_deps")

go_rules_dependencies()

go_register_toolchains(version = GO_VERSION)

# gazelle:repository_macro package.bzl%go_deps
go_deps()

## Protobuf dependencies.

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

## Gazelle dependencies.

load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies")

# gazelle:repo bazel_gazelle
gazelle_dependencies()

# Rust dependencies.

load("@rules_rust//crate_universe:repositories.bzl", "crate_universe_dependencies")
load("@rules_rust//rust:repositories.bzl", "rules_rust_dependencies", "rust_register_toolchains")
load("@rules_rust//tools/rust_analyzer:deps.bzl", "rust_analyzer_dependencies")

rules_rust_dependencies()

rust_register_toolchains(edition = RUST_EDITION)

crate_universe_dependencies(bootstrap = True)

rust_analyzer_dependencies()

load("@rules_rust//crate_universe:defs.bzl", "crates_repository")

crates_repository(
    name = "crate_index",
    cargo_lockfile = "//:Cargo.lock",
    lockfile = "//:Cargo.bazel.lock",
    manifests = ["//:Cargo.toml"],
)

load("@crate_index//:defs.bzl", "crate_repositories")

crate_repositories()

## Skylib dependencies.

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

## Fuzzing dependencies.
load("@rules_fuzzing//fuzzing:repositories.bzl", "rules_fuzzing_dependencies")

rules_fuzzing_dependencies()

load("@rules_fuzzing//fuzzing:init.bzl", "rules_fuzzing_init")

rules_fuzzing_init()

## C/C++ dependencies (foreign toolchains).

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies(
    register_preinstalled_tools = False,
)

## C/C++ dependencies (external).

deps()
