package(default_visibility = ["//visibility:public"])

cc_library(
    name = "node-addon-api",
    hdrs = glob([
      "include/**/*.h",
      "include/**/*.hpp",
    ]),
    deps = [
      "//libs/node-14.8.0:node",
    ],
    strip_include_prefix = "include",
)
