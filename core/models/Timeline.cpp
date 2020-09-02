#include <models/Timeline.h>

#include <models/Utils.h>

namespace tlp {

rapidjson::Value::StringRefType toStringRef(InterpMethod m) {
  switch (m) {
    case InterpMethod::NO_INTERP:
      return "NO_INTERP";
    case InterpMethod::LINEAR:
      return "LINEAR";
    case InterpMethod::QUADRATIC:
      return "QUADRATIC";
    case InterpMethod::CUBIC:
      return "CUBIC";
    default:
      // Unreachable.
      assert(false);
      return "Unknown";
  }
}

rapidjson::Value Keyframe::toJson(JsonAlloc& allocator) const {
  rapidjson::Value val;
  val.SetObject()
      .AddMember("ref_image_id", _refImageId, allocator)
      .AddMember("crop_rect", rect2dToJson(_cropRect, allocator), allocator)
      .AddMember("ev_delta", _evDelta, allocator)
      .AddMember("interp_method", toStringRef(_interpMethod), allocator);
  return val;
}

Keyframe Keyframe::fromJson(const rapidjson::Value& json) {
  // TODO: Implement this.
  return Keyframe();
}

rapidjson::Value Timeline::toJson(JsonAlloc& allocator) const {
  rapidjson::Value keyframesJson;
  keyframesJson.SetArray();
  for (const auto [t, kf] : _keyframes) {
    keyframesJson.PushBack(kf.toJson(allocator), allocator);
  }

  rapidjson::Value val;
  val.SetObject().AddMember("keyframes", keyframesJson, allocator);
  return val;
}

Timeline Timeline::fromJson(const rapidjson::Value& json) {
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