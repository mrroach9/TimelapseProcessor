#include <models/Utils.h>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <iostream>

namespace tlp {

rapidjson::Value mat3dToJson(const cv::Mat& mat, JsonAlloc& allocator) {
  assert(mat.rows == 3 && mat.cols == 3 && mat.type() == CV_64F);
  rapidjson::Value val;
  val.SetArray();
  for (size_t i = 0; i < 3; ++i) {
    rapidjson::Value row;
    row.SetArray();
    for (size_t j = 0; j < 3; ++j) {
      row.PushBack(mat.at<double>(i, j), allocator);
    }
    val.PushBack(row, allocator);
  }
  return val;
}

tl::expected<cv::Mat, Error> mat3dFromJson(const rapidjson::Value& json) {
  cv::Mat m(3, 3, CV_64F);
  if (!json.IsArray()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "Node is not an array!"});
  } else if (json.Size() != 3) {
    return tl::unexpected(Error{
        ErrorCode::JSON_ARRAY_WRONG_SIZE, "Array is not of size 3!"});
  }
  for (size_t r = 0; r < 3; ++r) {
    const rapidjson::Value& rowJson = json[r];
    if (!rowJson.IsArray()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE,
          "Element #" + std::to_string(r) + " of array node is not an array!"});
    } else if (rowJson.Size() != 3) {
      return tl::unexpected(Error{
          ErrorCode::JSON_ARRAY_WRONG_SIZE,
          "Element #" + std::to_string(r) + " of array node is not of size 3!"});
    }
    for (size_t c = 0; c < 3; ++c) {
      const rapidjson::Value& elemJson = rowJson[c];
      if (!elemJson.IsDouble()) {
        return tl::unexpected(Error{
            ErrorCode::JSON_WRONG_NODE_TYPE,
            "Element (" + std::to_string(r) + ", " + std::to_string(c) +
            ") of array node is not a double!"});
      }
      m.at<double>(r, c) = elemJson.GetDouble();
    }
  }
  return m;
}

rapidjson::Value::StringRefType toStringRef(const std::string& str) {
  return rapidjson::StringRef(str.c_str());
}

std::string toString(const rapidjson::Document& d, bool pretty) {
  rapidjson::StringBuffer buffer;
  if (pretty) {
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
  } else {
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
  }
  return buffer.GetString();
}

std::string toString(const rapidjson::Value& d, bool pretty) {
  rapidjson::StringBuffer buffer;
  if (pretty) {
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
  } else {
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);
  }
  return buffer.GetString();
}

rapidjson::Value rect2dToJson(const cv::Rect2d& rect, JsonAlloc& allocator) {
  rapidjson::Value val;
  val.SetArray()
      .PushBack(rect.x, allocator)
      .PushBack(rect.y, allocator)
      .PushBack(rect.width, allocator)
      .PushBack(rect.height, allocator);
  return val;
}

tl::expected<size_t, Error> getUintFromJsonChild(
    const rapidjson::Value& json,
    const std::string& fieldName) {
  if (json.HasMember(fieldName.c_str())) {
    const auto& child = json[fieldName.c_str()];
    if (!child.IsUint()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE, fieldName + " is not a uint!"});
    }
    return child.GetUint();
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, fieldName + " field is missing!"
    });
  }
}

}