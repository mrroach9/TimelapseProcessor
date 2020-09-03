#include <models/Utils.h>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace tlp {

rapidjson::Value mat3dToJson(const cv::Mat& mat, JsonAlloc& allocator) {
  assert(mat.rows == 3 && mat.cols == 3 && mat.type() == CV_32F);
  rapidjson::Value val;
  val.SetArray();
  for (size_t i = 0; i < 3; ++i) {
    rapidjson::Value row;
    row.SetArray();
    for (size_t j = 0; j < 3; ++j) {
      row.PushBack(mat.at<float>(i, j), allocator);
    }
    val.PushBack(row, allocator);
  }
  return val;
}

cv::Mat mat3dFromJson(const rapidjson::Value& json) {
  // TODO: implement this.
  return cv::Mat();
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

}