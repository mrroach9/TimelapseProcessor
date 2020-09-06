#pragma once

#include <string>

namespace tlp {

enum class ErrorCode {
  JSON_WRONG_NODE_TYPE = 1,
  JSON_MISSING_FIELD   = 2,
  JSON_ARRAY_WRONG_SIZE = 3,
};

struct Error {
  ErrorCode code;
  std::string msg;
};

}