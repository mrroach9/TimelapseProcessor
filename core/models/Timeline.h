#pragma once

#include <models/Image.h>

#include <opencv2/core/types.hpp>
 
#include <chrono>
#include <map>
#include <string>
#include <vector>

namespace tlp {
namespace chr = std::chrono;
using TimePoint = chr::system_clock::time_point;

enum class InterpMethod {
  NO_INTERP,
  BILINEAR,
  BIQUADRATIC,
  BICUBIC,
};

class Keyframe {
public:
  std::string toJson() const;
  static Keyframe fromJson(const std::string& json);

private:
  Image _refImage;
  cv::Rect2d _cropRect;
  double _evDelta;
  // Interpolation method between the previous keyframe and this one.
  // The first keyframe in a timeline must set this to NO_INTERP.
  InterpMethod _interpMethod; 
};

class Timeline {
public:
  std::string toJson() const;
  static Timeline fromJson(const std::string& json);

  int addImages(const std::vector<Image>& images);
  int addKeyframes(const std::vector<Keyframe>& keyframes);
  int addKeyframe(const Keyframe& keyframe);

private:
  // A list of all imported images keyed by their real time of shooting extracted
  // from EXIF.
  std::map<TimePoint, Image> _images;
  // A list of all keyframes keyed by their timestamp. Note that each keyframe
  // must reference to an input image, hence timestamp must be identical to 
  // an element in _images.
  std::map<TimePoint, Keyframe> _keyframes;
};

}