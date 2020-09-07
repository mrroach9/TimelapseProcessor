#pragma once

#include <string>

namespace tlp {

enum class ErrorCode {
  JSON_WRONG_NODE_TYPE = 1,
  JSON_MISSING_FIELD   = 2,
  JSON_ARRAY_WRONG_SIZE = 3,
  JSON_ENUM_INVALID_VALUE = 4,

  MODEL_KEYFRAME_REF_NONEXIST_IMG = 10001,
  MODEL_KEYFRAME_WITH_SAME_TIME_EXISTS = 10002,
  MODEL_IMAGE_WITH_SAME_ID_EXISTS = 10003,
};

struct Error {
  ErrorCode code;
  std::string msg;
};

}