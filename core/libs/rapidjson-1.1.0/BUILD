package(default_visibility = ["//visibility:public"])

cc_library(
    name = "rapidjson",
    hdrs = glob([
      "include/**/*.h",
      "include/**/*.hpp",
    ]),
    strip_include_prefix = "include",
    defines = [
      "RAPIDJSON_HAS_STDSTRING=1",
    ]
)
