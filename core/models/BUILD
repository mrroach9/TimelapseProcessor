package(default_visibility = ["//visibility:public"])

cc_library(
    name = "models",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.h"]),
    deps = [
      "//common:error",
      "//common:math",
      "//common:utils",
      "//libs/opencv-4.4.0:opencv",
      "//libs/rapidjson-1.1.0:rapidjson",
      "//libs/expected-1.0:expected",
    ],
)

cc_test(
    name = "model_serialization_tests",
    srcs = [
      "tests/ImageTest.cpp",
      "tests/ProjectTest.cpp",
      "tests/TimelineTest.cpp",
    ],
    deps = [
      ":models",
      "//libs/googletest-v1.10.0:googletest",
    ],
)
