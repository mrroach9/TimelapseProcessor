package(default_visibility = ["//visibility:public"])

cc_library(
    name = "models",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    deps = [],
)

cc_test(
    name = "project_test",
    srcs = ["tests/ProjectTest.cpp"],
    deps = [
      ":models",
      "//libs/googletest-v1.10.0:googletest",
    ],
)
