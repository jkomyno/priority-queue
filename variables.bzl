COPTS = select({
    "@bazel_tools//src/conditions:windows": ["/std:c++17", "/W4", "/Oi", "/Ot", "/WX"],
    "//conditions:default": ["-std=c++17", "-O3", "-Wall", "-Wextra"],
})
