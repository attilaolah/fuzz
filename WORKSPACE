workspace(name = "fuzz")

## Workspace dependencies & toolchains.

load("//:versions.bzl", "GO_VERSION", "RUST_EDITION")
load("//:workspace.bzl", "workspace_dependencies")

workspace_dependencies()

register_toolchains("//toolchains/cc:all")

## C/C++ dependencies (foreign toolchains).

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies(
    register_preinstalled_tools = False,
)

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

load("@bazel_gazelle//:deps.bzl", "gazelle_dependencies", "go_repository")

go_repository(
    name = "com_github_aws_aws_sdk_go_v2",
    importpath = "github.com/aws/aws-sdk-go-v2",
    sum = "h1:0W1TSJ7O6OzwuEvIXAtJGvOeQ0SGAhcpxPN2/NK5EhM=",
    version = "v1.16.3",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_aws_protocol_eventstream",
    importpath = "github.com/aws/aws-sdk-go-v2/aws/protocol/eventstream",
    sum = "h1:SdK4Ppk5IzLs64ZMvr6MrSficMtjY2oS0WOORXTlxwU=",
    version = "v1.4.1",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_config",
    importpath = "github.com/aws/aws-sdk-go-v2/config",
    sum = "h1:P+xwhr6kabhxDTXTVH9YoHkqjLJ0wVVpIUHtFNr2hjU=",
    version = "v1.15.5",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_credentials",
    importpath = "github.com/aws/aws-sdk-go-v2/credentials",
    sum = "h1:4R/NqlcRFSkR0wxOhgHi+agGpbEr5qMCjn7VqUIJY+E=",
    version = "v1.12.0",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_feature_ec2_imds",
    importpath = "github.com/aws/aws-sdk-go-v2/feature/ec2/imds",
    sum = "h1:FP8gquGeGHHdfY6G5llaMQDF+HAf20VKc8opRwmjf04=",
    version = "v1.12.4",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_feature_s3_manager",
    importpath = "github.com/aws/aws-sdk-go-v2/feature/s3/manager",
    sum = "h1:JL7cY85hyjlgfA29MMyAlItX+JYIH9XsxgMBS7jtlqA=",
    version = "v1.11.10",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_internal_configsources",
    importpath = "github.com/aws/aws-sdk-go-v2/internal/configsources",
    sum = "h1:uFWgo6mGJI1n17nbcvSc6fxVuR3xLNqvXt12JCnEcT8=",
    version = "v1.1.10",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_internal_endpoints_v2",
    importpath = "github.com/aws/aws-sdk-go-v2/internal/endpoints/v2",
    sum = "h1:cnsvEKSoHN4oAN7spMMr0zhEW2MHnhAVpmqQg8E6UcM=",
    version = "v2.4.4",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_internal_ini",
    importpath = "github.com/aws/aws-sdk-go-v2/internal/ini",
    sum = "h1:6cZRymlLEIlDTEB0+5+An6Zj1CKt6rSE69tOmFeu1nk=",
    version = "v1.3.11",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_internal_v4a",
    importpath = "github.com/aws/aws-sdk-go-v2/internal/v4a",
    sum = "h1:C21IDZCm9Yu5xqjb3fKmxDoYvJXtw1DNlOmLZEIlY1M=",
    version = "v1.0.1",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_service_internal_accept_encoding",
    importpath = "github.com/aws/aws-sdk-go-v2/service/internal/accept-encoding",
    sum = "h1:T4pFel53bkHjL2mMo+4DKE6r6AuoZnM0fg7k1/ratr4=",
    version = "v1.9.1",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_service_internal_checksum",
    importpath = "github.com/aws/aws-sdk-go-v2/service/internal/checksum",
    sum = "h1:9LSZqt4v1JiehyZTrQnRFf2mY/awmyYNNY/b7zqtduU=",
    version = "v1.1.5",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_service_internal_presigned_url",
    importpath = "github.com/aws/aws-sdk-go-v2/service/internal/presigned-url",
    sum = "h1:b16QW0XWl0jWjLABFc1A+uh145Oqv+xDcObNk0iQgUk=",
    version = "v1.9.4",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_service_internal_s3shared",
    importpath = "github.com/aws/aws-sdk-go-v2/service/internal/s3shared",
    sum = "h1:RE/DlZLYrz1OOmq8F28IXHLksuuvlpzUbvJ+SESCZBI=",
    version = "v1.13.4",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_service_s3",
    importpath = "github.com/aws/aws-sdk-go-v2/service/s3",
    sum = "h1:LCQKnopq2t4oQS3VKivlYTzAHCTJZZoQICM9fny7KHY=",
    version = "v1.26.9",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_service_sso",
    importpath = "github.com/aws/aws-sdk-go-v2/service/sso",
    sum = "h1:Uw5wBybFQ1UeA9ts0Y07gbv0ncZnIAyw858tDW0NP2o=",
    version = "v1.11.4",
)

go_repository(
    name = "com_github_aws_aws_sdk_go_v2_service_sts",
    importpath = "github.com/aws/aws-sdk-go-v2/service/sts",
    sum = "h1:+xtV90n3abQmgzk1pS++FdxZTrPEDgQng6e4/56WR2A=",
    version = "v1.16.4",
)

go_repository(
    name = "com_github_aws_smithy_go",
    importpath = "github.com/aws/smithy-go",
    sum = "h1:eG/N+CcUMAvsdffgMvjMKwfyDzIkjM6pfxMJ8Mzc6mE=",
    version = "v1.11.2",
)

go_repository(
    name = "com_github_azure_azure_sdk_for_go_sdk_azcore",
    importpath = "github.com/Azure/azure-sdk-for-go/sdk/azcore",
    sum = "h1:Ut0ZGdOwJDw0npYEg+TLlPls3Pq6JiZaP2/aGKir7Zw=",
    version = "v1.1.0",
)

go_repository(
    name = "com_github_azure_azure_sdk_for_go_sdk_azidentity",
    importpath = "github.com/Azure/azure-sdk-for-go/sdk/azidentity",
    sum = "h1:QkAcEIAKbNL4KoFr4SathZPhDhF4mVwpBMFlYjyAqy8=",
    version = "v1.1.0",
)

go_repository(
    name = "com_github_azure_azure_sdk_for_go_sdk_internal",
    importpath = "github.com/Azure/azure-sdk-for-go/sdk/internal",
    sum = "h1:jp0dGvZ7ZK0mgqnTSClMxa5xuRL7NZgHameVYF6BurY=",
    version = "v1.0.0",
)

go_repository(
    name = "com_github_azure_azure_sdk_for_go_sdk_storage_azblob",
    importpath = "github.com/Azure/azure-sdk-for-go/sdk/storage/azblob",
    sum = "h1:QSdcrd/UFJv6Bp/CfoVf2SrENpFn9P6Yh8yb+xNhYMM=",
    version = "v0.4.1",
)

go_repository(
    name = "com_github_azuread_microsoft_authentication_library_for_go",
    importpath = "github.com/AzureAD/microsoft-authentication-library-for-go",
    sum = "h1:XMEdVDFxgulDDl0lQmAZS6j8gRQ/0pJ+ZpXH2FHVtDc=",
    version = "v0.6.0",
)

go_repository(
    name = "com_github_containerd_nydus_snapshotter",
    importpath = "github.com/containerd/nydus-snapshotter",
    sum = "h1:b8WahTrPkt3XsabjG2o/leN4fw3HWZYr+qxo/Z8Mfzk=",
    version = "v0.3.1",
)

go_repository(
    name = "com_github_in_toto_in_toto_golang",
    importpath = "github.com/in-toto/in-toto-golang",
    sum = "h1:hb8bgwr0M2hGdDsLjkJ3ZqJ8JFLL/tgYdAxF/XEFBbY=",
    version = "v0.5.0",
)

go_repository(
    name = "com_github_jmespath_go_jmespath",
    importpath = "github.com/jmespath/go-jmespath",
    sum = "h1:BEgLn5cpjn8UN1mAw4NjwDrS35OdebyEtFe+9YPoQUg=",
    version = "v0.4.0",
)

go_repository(
    name = "com_github_kylelemons_godebug",
    importpath = "github.com/kylelemons/godebug",
    sum = "h1:RPNrshWIDI6G2gRW9EHilWtl7Z6Sb1BR0xunSBf0SNc=",
    version = "v1.1.0",
)

go_repository(
    name = "com_github_moby_patternmatcher",
    importpath = "github.com/moby/patternmatcher",
    sum = "h1:YCZgJOeULcxLw1Q+sVR636pmS7sPEn1Qo2iAN6M7DBo=",
    version = "v0.5.0",
)

go_repository(
    name = "com_github_moby_sys_sequential",
    importpath = "github.com/moby/sys/sequential",
    sum = "h1:OPvI35Lzn9K04PBbCLW0g4LcFAJgHsvXsRyewg5lXtc=",
    version = "v0.5.0",
)

go_repository(
    name = "com_github_package_url_packageurl_go",
    importpath = "github.com/package-url/packageurl-go",
    sum = "h1:DiLBVp4DAcZlBVBEtJpNWZpZVq0AEeCY7Hqk8URVs4o=",
    version = "v0.1.1-0.20220428063043-89078438f170",
)

go_repository(
    name = "com_github_pkg_browser",
    importpath = "github.com/pkg/browser",
    sum = "h1:Qj1ukM4GlMWXNdMBuXcXfz/Kw9s1qm0CLY32QxuSImI=",
    version = "v0.0.0-20210115035449-ce105d075bb4",
)

go_repository(
    name = "com_github_secure_systems_lab_go_securesystemslib",
    importpath = "github.com/secure-systems-lab/go-securesystemslib",
    sum = "h1:b23VGrQhTA8cN2CbBw7/FulN9fTtqYUdS5+Oxzt+DUE=",
    version = "v0.4.0",
)

go_repository(
    name = "com_github_shibumi_go_pathspec",
    importpath = "github.com/shibumi/go-pathspec",
    sum = "h1:QUyMZhFo0Md5B8zV8x2tesohbb5kfbpTi9rBnKh5dkI=",
    version = "v1.3.0",
)

go_repository(
    name = "com_github_spdx_tools_golang",
    importpath = "github.com/spdx/tools-golang",
    sum = "h1:9B623Cfs+mclYK6dsae7gLSwuIBHvlgmEup87qpqsAQ=",
    version = "v0.3.1-0.20230104082527-d6f58551be3f",
)

go_repository(
    name = "org_golang_x_term",
    importpath = "golang.org/x/term",
    sum = "h1:CBpWXWQpIRjzmkkA+M7q9Fqnwd2mZr3AFqexg8YTfoM=",
    version = "v0.0.0-20220526004731-065cf7ba2467",
)

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

## C/C++ dependencies (external).

deps()
