#include <models/Project.h>

#include <models/Utils.h>

namespace tlp {

rapidjson::Value::StringRefType toStringRef(VideoEncoding o) {
  switch (o) {
    case VideoEncoding::H264:
      return "H264";
    case VideoEncoding::SEQ_OF_IMAGES:
      return "SEQ_OF_IMAGES";
    default:
      assert(false);
      return "Unreachable";
  }
}

tl::expected<VideoEncoding, Error> videoEncodingfromStringRef(
    const rapidjson::Value::StringRefType& str) {
  if (str == "H264") {
    return VideoEncoding::H264;
  } else if (str == "SEQ_OF_IMAGES") {
    return VideoEncoding::SEQ_OF_IMAGES;
  } else {
    return tl::unexpected(Error{
        ErrorCode::JSON_ENUM_INVALID_VALUE,
        "Invalid value for VideoEncoding: " + std::string(str)});
  }
}

rapidjson::Value::StringRefType toStringRef(VideoResolution o) {
  switch (o) {
    case VideoResolution::RES_480P:
      return "480p";
    case VideoResolution::RES_720P:
      return "720p";
    case VideoResolution::RES_1080P:
      return "1080p";
    case VideoResolution::RES_2160P:
      return "2160P";
    default:
      assert(false);
      return "Unreachable";
  }
}

tl::expected<VideoResolution, Error> videoResolutionfromStringRef(
    const rapidjson::Value::StringRefType& str) {
  if (str == "480p") {
    return VideoResolution::RES_480P;
  } else if (str == "720p") {
    return VideoResolution::RES_720P;
  } else if (str == "1080p") {
    return VideoResolution::RES_1080P;
  } else if (str == "2160p") {
    return VideoResolution::RES_2160P;
  } else {
    return tl::unexpected(Error{
        ErrorCode::JSON_ENUM_INVALID_VALUE,
        "Invalid value for VideoResolution: " + std::string(str)});
  }
}

Project::Project(const std::string& name,
    VideoEncoding encoding,
    VideoResolution resolution,
    const std::vector<Image>& images,
    const Timeline& timeline) :
    _projectName(name),
    _exportEncoding(encoding),
    _exportResolution(resolution),
    _timeline(timeline) {
  for (const Image& image : images) {
    _imagesById[image.id()] = image;
    _imagesByFilepath[image.filepath()] = image;
  }
}

rapidjson::Value Project::toJson(JsonAlloc& allocator) const {
  rapidjson::Value imagesJson;
  imagesJson.SetArray();
  for (const auto& [id, img] : _imagesById) {
    imagesJson.PushBack(img.toJson(allocator), allocator);
  }

  rapidjson::Value val;
  val.SetObject()
      .AddMember("project_name", toStringRef(_projectName), allocator)
      .AddMember("export_encoding", toStringRef(_exportEncoding), allocator)
      .AddMember("export_resolution", toStringRef(_exportResolution), allocator)
      .AddMember("timeline", _timeline.toJson(allocator), allocator)
      .AddMember("images", imagesJson, allocator);
  return val;
}

Project Project::fromJson(const rapidjson::Value& json) {
  // TODO: Implement it.
  return Project();
}

bool operator==(const Project& a, const Project& b) {
  return a._projectName == b._projectName
      && a._exportEncoding == b._exportEncoding
      && a._exportResolution == b._exportResolution
      && a._timeline == b._timeline
      && std::equal(a._imagesById.begin(), a._imagesById.end(), b._imagesById.begin());
}

}