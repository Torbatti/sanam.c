const std = @import("std");
const builtin = @import("builtin");
const assert = std.debug.assert;

const zig_version = std.SemanticVersion{
    .major = 0,
    .minor = 15,
    .patch = 1,
};

comptime {
    // Compare versions while allowing different pre/patch metadata.
    const zig_version_eq = zig_version.major == builtin.zig_version.major and
        zig_version.minor == builtin.zig_version.minor and
        zig_version.patch == builtin.zig_version.patch;
    if (!zig_version_eq) {
        @compileError(std.fmt.comptimePrint(
            "unsupported zig version: expected {}, found {}",
            .{ zig_version, builtin.zig_version },
        ));
    }
}

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "sanam",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        }),
    });

    exe.addCSourceFile(.{ .file = b.path("src/sanam.c") });

    //
    switch (target.result.os.tag) {
        .windows => {
            switch (target.result.cpu.arch) {
                .x86_64, .aarch64 => {},
                else => {
                    @panic("target cpu is not planned to be supported/tested");
                },
            }
        },

        .linux => {
            switch (target.result.cpu.arch) {
                .x86_64, .riscv64, .aarch64 => {},
                else => {
                    @panic("target cpu is not planned to be supported/tested");
                },
            }
        },

        .macos, .freebsd, .openbsd, .netbsd => {
            @panic("target os is planned but not implemented yet");
        },

        else => {
            @panic("target os is not planned to be supported/tested");
        },
    }

    b.installArtifact(exe);
}
