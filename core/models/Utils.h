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

rapidjson::Value::StringRefType toStringRef(const std::string& str);

rapidjson::Value rect2dToJson(const cv::Rect2d& rect, JsonAlloc& allocator);

std::string toString(const rapidjson::Document& d, bool pretty = false);

std::string toString(const rapidjson::Value& d, bool pretty = false);

tl::expected<size_t, Error> getUintFromJsonChild(
    const rapidjson::Value& json,
    const std::string& fieldName);
}