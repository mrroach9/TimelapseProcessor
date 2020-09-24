#pragma once

#include <common/Error.h>
#include <models/TypeTraits.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <rapidjson/document.h>
#include <tl/expected.hpp>

#include <string>

namespace tlp {

rapidjson::Value mat3dToJson(const cv::Mat& mat, JsonAlloc& allocator);

tl::expected<cv::Mat, Error> mat3dFromJson(const rapidjson::Value& json);

rapidjson::Value rect2dToJson(const cv::Rect2d& rect, JsonAlloc& allocator);

tl::expected<cv::Rect2d, Error> rect2dFromJson(const rapidjson::Value& json);

std::string toString(const rapidjson::Document& d, bool pretty = false);

std::string toString(const rapidjson::Value& d, bool pretty = false);

template <typename T>
tl::expected<T, Error> getValueFromJsonChild(
    const rapidjson::Value& json,
    const std::string& fieldName) {
  if (json.HasMember(fieldName.c_str())) {
    const auto& child = json[fieldName.c_str()];
    if (!child.Is<T>()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE,
          fieldName + " is not of type " + typeid(T).name()});
    }
    return child.Get<T>();
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, fieldName + " field is missing!"
    });
  }
}

tl::expected<std::vector<double>, Error> getDoubleArrayFromJson(
    const rapidjson::Value& json,
    size_t size);

}