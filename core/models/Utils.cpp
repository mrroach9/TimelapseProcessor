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
    const auto maybeRow = getDoubleArrayFromJson(json[r], 3);
    if (maybeRow) {
      for (size_t c = 0; c < 3; ++c) {
        m.at<double>(r, c) = maybeRow.value()[c];
      }
    } else {
      return tl::unexpected(maybeRow.error());
    }
  }
  return m;
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

tl::expected<cv::Rect2d, Error> rect2dFromJson(const rapidjson::Value& json) {
  cv::Rect2d r;
  const auto maybeArr = getDoubleArrayFromJson(json, 4);
  if (maybeArr) {
    const std::vector<double> arr = maybeArr.value();
    return cv::Rect2d(arr[0], arr[1], arr[2], arr[3]);
  } else {
    return tl::unexpected(maybeArr.error());
  }
}

tl::expected<std::vector<double>, Error> getDoubleArrayFromJson(
    const rapidjson::Value& json, size_t size) {
  if (!json.IsArray()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "Node is not an array!"});
  } else if (json.Size() != size) {
    return tl::unexpected(Error{
        ErrorCode::JSON_ARRAY_WRONG_SIZE, "Array is not of size " + std::to_string(size)});
  }
  std::vector<double> arr;
  for (size_t i = 0; i < size; ++i) {
    if (!json[i].IsDouble()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE,
          "Element " + std::to_string(i) + " is not a double!"});
    }
    arr.push_back(json[i].GetDouble());
  }
  return arr;
}

}