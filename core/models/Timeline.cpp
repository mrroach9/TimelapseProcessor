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

tl::expected<InterpMethod, Error> interpMethodFromString(const std::string& str) {
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

tl::expected<Keyframe, Error> Keyframe::fromJson(const rapidjson::Value& json) {
  Keyframe keyframe;
  if (!json.IsObject()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "Keyframe JSON node is not an object!"});
  }

  const auto maybeId = getUintFromJsonChild(json, "ref_image_id");
  if (!maybeId) {
    return tl::unexpected(maybeId.error());
  } else {
    keyframe._refImageId = maybeId.value();
  }

  if (json.HasMember("crop_rect")) {
    const auto maybeCropRect = rect2dFromJson(json["crop_rect"]);
    if (maybeCropRect) {
      keyframe._cropRect = maybeCropRect.value();
    } else {
      return tl::unexpected(maybeCropRect.error());
    }
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "crop_rect field is missing!"
    });
  }

  if (json.HasMember("ev_delta")) {
    const auto& evDeltaJson = json["ev_delta"];
    if (evDeltaJson.IsDouble()) {
      keyframe._evDelta = evDeltaJson.GetDouble();
    } else {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE, "ev_delta is not a double!"});
    }
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "ev_delta field is missing!"
    });
  }

  if (json.HasMember("interp_method")) {
    const auto& interpMethodJson = json["interp_method"];
    if (!interpMethodJson.IsString()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE, "interp_method is not a string!"});
    }
    const auto maybeInterpMethod = interpMethodFromString(interpMethodJson.GetString());
    if (maybeInterpMethod) {
      keyframe._interpMethod = maybeInterpMethod.value();
    } else {
      return tl::unexpected(maybeInterpMethod.error());
    }
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "interp_method field is missing!"
    });
  }
  return keyframe;
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
  for (const auto& kf : _keyframes) {
    keyframesJson.PushBack(kf.toJson(allocator), allocator);
  }

  rapidjson::Value val;
  val.SetObject().AddMember("keyframes", keyframesJson, allocator);
  return val;
}

tl::expected<Timeline, Error> Timeline::fromJson(const rapidjson::Value& json) {
  Timeline tl;
  if (!json.IsObject()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "Timeline JSON node is not an object!"});
  } else if (!json.HasMember("keyframes")) {
    return tl::unexpected(Error{
        ErrorCode::JSON_MISSING_FIELD, "keyframes field is missing!"});
  } else if (!json["keyframes"].IsArray()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "keyframe is not an array!"});
  }
  for (const auto& keyframeJson : json["keyframes"].GetArray()) {
    const auto maybeKeyframe = Keyframe::fromJson(keyframeJson);
    if (maybeKeyframe) {
      tl._keyframes.push_back(maybeKeyframe.value());
    } else {
      return tl::unexpected(maybeKeyframe.error());
    }
  }
  return tl;
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
  return std::equal(a._keyframes.begin(), a._keyframes.end(), b._keyframes.begin());
}

}