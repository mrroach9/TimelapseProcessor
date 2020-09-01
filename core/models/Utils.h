#pragma once

#include <models/TypeTraits.h>

#include <opencv2/core/mat.hpp>
#include <rapidjson/document.h>

#include <string>

namespace tlp {

rapidjson::Value mat3dToJson(const cv::Mat& mat, JsonAlloc& allocator);

cv::Mat mat3dFromJson(const rapidjson::Value& json);

rapidjson::Value::StringRefType toStringRef(const std::string& str);

rapidjson::Value::StringRefType toStringRef(const fs::path& path);

std::string toString(const rapidjson::Document& d, bool pretty = false);

std::string toString(const rapidjson::Value& d, bool pretty = false);
}