#pragma once

#include <common/Error.h>
#include <models/Image.h>
#include <models/Timeline.h>
#include <models/TypeTraits.h>

#include <rapidjson/document.h>
#include <tl/expected.hpp>

#include <map>

namespace tlp {

enum class VideoEncoding {
  H264,
  SEQ_OF_IMAGES
};

enum class VideoResolution {
  RES_480P,
  RES_720P,
  RES_1080P,
  RES_2160P
};

rapidjson::Value::StringRefType toStringRef(VideoEncoding o);
tl::expected<VideoEncoding, Error> videoEncodingfromStringRef(
    const rapidjson::Value::StringRefType& str);

rapidjson::Value::StringRefType toStringRef(VideoResolution o);
tl::expected<VideoResolution, Error> videoResolutionfromStringRef(
    const rapidjson::Value::StringRefType& str);

class Project {
public:
  // TODO: Delete these explicit constructors when parsing feature is added. Temporarily
  // added for testing purpose.
  Project(const std::string& name,
      VideoEncoding encoding,
      VideoResolution resolution,
      const std::vector<Image>& images,
      const Timeline& timeline);
  Project() {}

  rapidjson::Value toJson(JsonAlloc& allocator) const;
  static Project fromJson(const rapidjson::Value& json);

  friend bool operator==(const Project& a, const Project& b);

private:
  std::map<size_t, Image> _imagesById;
  std::map<std::string, Image> _imagesByFilepath;
  Timeline _timeline;
  std::string _projectName;
  VideoEncoding _exportEncoding;
  VideoResolution _exportResolution;
};

bool operator==(const Project& a, const Project& b);

}