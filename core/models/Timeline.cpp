#include <models/Timeline.h>

namespace tlp {

std::string Keyframe::toJson() const {
  // TODO: Implement this.
  return "Keyframe";
}

Keyframe Keyframe::fromJson(const std::string& json) {
  // TODO: Implement this.
  return Keyframe();
}

std::string Timeline::toJson() const {
  // TODO: Implement this.
  return "Timeline";
}

Timeline Timeline::fromJson(const std::string& json) {
  // TODO: Implement this.
  return Timeline();
}

int Timeline::addImages(const std::vector<Image>& images) {
  // TODO: Implement this.
  return 0;
}

int Timeline::addKeyframes(const std::vector<Keyframe>& keyframes) {
  // TODO: Implement this.
  return 0;
}

int Timeline::addKeyframe(const Keyframe& keyframe) {
  // TODO: Implement this.
  return 0;
}

}