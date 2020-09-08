#pragma once

#include <common/Error.h>

#include <tl/expected.hpp>

#include <optional>

namespace tlp {

template <typename T, typename... Args>
inline std::optional<Error> aggregateErrors(const tl::expected<T, Error>& v1,
    const tl::expected<Args, Error>& ...args) {
  return v1 ? aggregateErrors(args...) : v1.error();
}

inline std::optional<Error> aggregateErrors() {
  return std::nullopt;
}

}