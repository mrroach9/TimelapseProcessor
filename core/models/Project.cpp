#include <models/Project.h>

#include <common/Utils.h>
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

tl::expected<VideoEncoding, Error> videoEncodingfromString(const std::string& str) {
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

tl::expected<VideoResolution, Error> videoResolutionfromString(const std::string& str) {
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

tl::expected<Project, Error> Project::fromJson(const rapidjson::Value& json) {
  Project project;
  if (!json.IsObject()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "Project JSON node is not an object!"});
  }
  const auto maybeProjectName = getValueFromJsonChild<const char*>(json, "project_name");
  const auto maybeExportEncoding =
      getValueFromJsonChild<const char*>(json, "export_encoding")
          .and_then(videoEncodingfromString);
  const auto maybeExportResolution =
      getValueFromJsonChild<const char*>(json, "export_resolution")
          .and_then(videoResolutionfromString);
  
  tl::expected<Timeline, Error> maybeTimeline;
  if (json.HasMember("timeline")) {
    maybeTimeline = Timeline::fromJson(json["timeline"]);
  } else {
    return tl::unexpected(Error{
        ErrorCode::JSON_MISSING_FIELD, "timeline field is missing!"});
  }

  if (const auto maybeError = aggregateErrors(
      maybeProjectName, maybeExportEncoding, maybeExportResolution, maybeTimeline)) {
    return tl::unexpected(*maybeError);
  }
  project._projectName = maybeProjectName.value();
  project._exportEncoding = maybeExportEncoding.value();
  project._exportResolution = maybeExportResolution.value();
  project._timeline = maybeTimeline.value();

  if (!json.HasMember("images")) {
    return tl::unexpected(Error{
        ErrorCode::JSON_MISSING_FIELD, "images field is missing!"});
  } else if (!json["images"].IsArray()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "images is not an array!"});
  }
  for (const auto& imageJson : json["images"].GetArray()) {
    const auto maybeImage = Image::fromJson(imageJson);
    if (maybeImage) {
      const Image& image = maybeImage.value();
      project._imagesById[image.id()] = image;
      project._imagesByFilepath[image.filepath()] = image;
      project._timeline.addImage(image);
    } else {
      return tl::unexpected(maybeImage.error());
    }
  }

  return project;
}

bool operator==(const Project& a, const Project& b) {
  return a._projectName == b._projectName
      && a._exportEncoding == b._exportEncoding
      && a._exportResolution == b._exportResolution
      && a._timeline == b._timeline
      && std::equal(a._imagesById.begin(), a._imagesById.end(), b._imagesById.begin());
}

}