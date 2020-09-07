#pragma once

#include <common/Error.h>
#include <models/Image.h>
#include <models/TypeTraits.h>

#include <opencv2/core/types.hpp>
#include <rapidjson/document.h>
#include <tl/expected.hpp>
 
#include <chrono>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace tlp {

enum class InterpMethod {
  NO_INTERP,
  LINEAR,
  QUADRATIC,
  CUBIC,
};

rapidjson::Value::StringRefType toStringRef(InterpMethod m);
tl::expected<InterpMethod, Error> interpMethodFromString(const std::string& str);

struct Keyframe {
  rapidjson::Value toJson(JsonAlloc& allocator) const;
  static tl::expected<Keyframe, Error> fromJson(const rapidjson::Value& json);

  size_t refImageId;
  cv::Rect2d cropRect;
  double evDelta;
  // Interpolation method between the previous keyframe and this one.
  // The first keyframe in a timeline must set this to NO_INTERP.
  InterpMethod interpMethod; 
};

bool operator==(const Keyframe& a, const Keyframe& b);

class Timeline {
public:
  // TODO: Delete these explicit constructors when parsing feature is added. Temporarily
  // added for testing purpose.
  Timeline(const std::vector<Keyframe>& keyframes) : _keyframes(keyframes) {}
  Timeline() {}

  rapidjson::Value toJson(JsonAlloc& allocator) const;
  static tl::expected<Timeline, Error> fromJson(const rapidjson::Value& json);

  std::optional<Error> addImage(const Image& image);
  std::optional<Error> addImages(const std::vector<Image>& images);
  std::optional<Error> addKeyframe(const Keyframe& keyframe);
  std::optional<Error> addKeyframes(const std::vector<Keyframe>& keyframes);

  friend bool operator==(const Timeline& a, const Timeline& b);

private:
  // A list of keyframes in this timeline. This is the only field serialized in the class.
  std::vector<Keyframe> _keyframes;

  // Temporary indices:

  // A list of all imported images keyed by their real time of shooting extracted
  // from EXIF.
  std::map<TimePoint, std::vector<Image>> _imagesByTime;
  // A list of all keyframes keyed by their timestamp. Note that each keyframe
  // must reference to an input image, hence timestamp must be identical to 
  // an element in _images.
  std::map<TimePoint, Keyframe> _keyframesByTime;
  // Cached all images by ids. The groundtruth should still be in Project.
  std::map<size_t, Image> _imagesById;
};

bool operator==(const Timeline& a, const Timeline& b);

}