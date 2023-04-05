def _to_proto_impl(ctx):
    outputs = []
    for src in ctx.files.srcs:
        output = ctx.actions.declare_file("{}.textpb".format(src.basename))

        args = ctx.actions.args()
        args.add("--gif", src)
        args.add("--proto", output)

        ctx.actions.run(
            executable = ctx.executable._to_proto,
            arguments = [args],
            inputs = [src],
            outputs = [output],
        )
        outputs.append(output)

    return DefaultInfo(files = depset(outputs))

to_proto = rule(
    implementation = _to_proto_impl,
    attrs = {
        "srcs": attr.label_list(
            doc = "Gif files to process.",
            allow_files = [".gif"],
            mandatory = True,
        ),
        "args": attr.string_list(
            doc = "List of args to pass on to the binary.",
            default = [],
        ),
        "_to_proto": attr.label(
            doc = "The to_proto binary.",
            default = "//src/bin:gif_to_proto",
            executable = True,
            cfg = "exec",
        ),
    },
)
