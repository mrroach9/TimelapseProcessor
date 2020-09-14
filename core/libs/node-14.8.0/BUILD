package(default_visibility = ["//visibility:public"])

cc_library(
    name = "node",
    srcs = select({
      "//build_defs:windows_x86_64": glob([
          "lib/windows_x86_64/**/*.lib",
      ]),
      "//conditions:default": [],
    }),
    hdrs = glob([
      "include/**/*.h",
      "include/**/*.hpp",
    ]),
    strip_include_prefix = "include",
)
