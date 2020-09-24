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

std::string toString(VideoEncoding o);
tl::expected<VideoEncoding, Error> videoEncodingfromString(const std::string& str);

std::string toString(VideoResolution o);
tl::expected<VideoResolution, Error> videoResolutionfromString(const std::string& str);

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
  static tl::expected<Project, Error> fromJson(const rapidjson::Value& json);

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