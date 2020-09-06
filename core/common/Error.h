#pragma once

#include <string>

namespace tlp {

enum class ErrorCode {
  JSON_WRONG_NODE_TYPE = 1,
  JSON_MISSING_FIELD   = 2,
  JSON_ARRAY_WRONG_SIZE = 3,
  JSON_ENUM_INVALID_VALUE = 4
};

struct Error {
  ErrorCode code;
  std::string msg;
};

}