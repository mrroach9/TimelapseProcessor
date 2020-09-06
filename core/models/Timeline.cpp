#include <models/Timeline.h>

#include <common/Math.h>
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
      assert(false);
      return "Unreachable";
  }
}

tl::expected<InterpMethod, Error> interpMethodFromStringRef(
    const rapidjson::Value::StringRefType& str) {
  if (str == "NO_INTERP") {
    return InterpMethod::NO_INTERP;
  } else if (str == "LINEAR") {
    return InterpMethod::LINEAR;
  } else if (str == "QUADRATIC") {
    return InterpMethod::QUADRATIC;
  } else if (str == "CUBIC") {
    return InterpMethod::CUBIC;
  } else {
    return tl::unexpected(Error{
        ErrorCode::JSON_ENUM_INVALID_VALUE,
        "Invalid value for InterpMethod: " + std::string(str)});
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

bool operator==(const Keyframe& a, const Keyframe& b) {
  return a._interpMethod == b._interpMethod
      && a._refImageId == b._refImageId
      && double_eq(a._evDelta, b._evDelta)
      && double_eq(a._cropRect.x, b._cropRect.x)
      && double_eq(a._cropRect.y, b._cropRect.y)
      && double_eq(a._cropRect.height, b._cropRect.height)
      && double_eq(a._cropRect.width, b._cropRect.width);
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

bool operator==(const Timeline& a, const Timeline& b) {
  return std::equal(a._keyframes.begin(), a._keyframes.end(), b._keyframes.begin())
      && std::equal(a._imagesById.begin(), a._imagesById.end(), b._imagesById.begin());
}

}