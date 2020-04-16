package(default_visibility = ["//visibility:public"])

load("@rules_cc//cc:defs.bzl", "cc_library") #, "cc_binary")
load("//:variables.bzl", "COPTS")

cc_library(
    name = "priority_queue",
    hdrs = glob(["*.h"]),
    copts = COPTS,
)

# cc_binary(
#     name = "priority_queue_main",
#     srcs = ["main.cpp"],
#     deps = [
#         ":priority_queue"
#     ],
#     copts = COPTS,
# )
