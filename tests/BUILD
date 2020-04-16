load("//:variables.bzl", "COPTS")

cc_test(
    name = "tests",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//priority_queue:priority_queue",
        "@googletest//:gtest_main",
    ],
    copts = COPTS,
)