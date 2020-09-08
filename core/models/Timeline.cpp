#include <models/Timeline.h>

#include <common/Math.h>
#include <common/Utils.h>
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
      .AddMember("ref_image_id", refImageId, allocator)
      .AddMember("crop_rect", rect2dToJson(cropRect, allocator), allocator)
      .AddMember("ev_delta", evDelta, allocator)
      .AddMember("interp_method", toStringRef(interpMethod), allocator);
  return val;
}

tl::expected<Keyframe, Error> Keyframe::fromJson(const rapidjson::Value& json) {
  Keyframe keyframe;
  if (!json.IsObject()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "Keyframe JSON node is not an object!"});
  }

  const auto maybeId = getValueFromJsonChild<size_t>(json, "ref_image_id");
  const auto maybeEvDelta = getValueFromJsonChild<double>(json, "ev_delta");
  const auto maybeInterp =
      getValueFromJsonChild<const char*>(json, "interp_method")
          .and_then(interpMethodFromString);

  tl::expected<cv::Rect2d, Error> maybeCropRect;
  if (json.HasMember("crop_rect")) {
    maybeCropRect = rect2dFromJson(json["crop_rect"]);
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "crop_rect field is missing!"
    });
  }

  if (const auto maybeError = aggregateErrors(maybeId, maybeEvDelta, maybeInterp)) {
    return tl::unexpected(*maybeError);
  }
  keyframe.refImageId = maybeId.value();
  keyframe.evDelta = maybeEvDelta.value();
  keyframe.interpMethod = maybeInterp.value();
  keyframe.cropRect = maybeCropRect.value();

  return keyframe;
}

bool operator==(const Keyframe& a, const Keyframe& b) {
  return a.interpMethod == b.interpMethod
      && a.refImageId == b.refImageId
      && double_eq(a.evDelta, b.evDelta)
      && double_eq(a.cropRect.x, b.cropRect.x)
      && double_eq(a.cropRect.y, b.cropRect.y)
      && double_eq(a.cropRect.height, b.cropRect.height)
      && double_eq(a.cropRect.width, b.cropRect.width);
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

std::optional<Error> Timeline::addImage(const Image& image) {
  if (_imagesById.count(image.id()) > 0) {
    return Error{ErrorCode::MODEL_IMAGE_WITH_SAME_ID_EXISTS,
        "Image exists with id = " + std::to_string(image.id())};
  }
  _imagesById[image.id()] = image;
  TimePoint tp = image.metadata().timestamp;
  if (_imagesByTime.count(tp) == 0) {
    _imagesByTime[tp] = std::vector<Image>{};
  }
  _imagesByTime[tp].push_back(image);
  return std::nullopt;
}

std::optional<Error> Timeline::addImages(const std::vector<Image>& images) {
  for (const auto& img : images) {
    if (auto res = addImage(img)) {
      return res;
    }
  }
  return std::nullopt;
}

std::optional<Error> Timeline::addKeyframes(const std::vector<Keyframe>& keyframes) {
  for (const auto& kf : keyframes) {
    if (auto res = addKeyframe(kf)) {
      return res;
    }
  }
  return std::nullopt;
}

std::optional<Error> Timeline::addKeyframe(const Keyframe& keyframe) {
  size_t refImgId = keyframe.refImageId;
  if (_imagesById.count(refImgId) == 0) {
    return Error{ErrorCode::MODEL_KEYFRAME_REF_NONEXIST_IMG,
        "Keyframe references non-existing image " + std::to_string(refImgId)};
  }
  TimePoint tp = _imagesById[refImgId].metadata().timestamp;
  if (_keyframesByTime.count(tp) > 0) {
    return Error{ErrorCode::MODEL_KEYFRAME_WITH_SAME_TIME_EXISTS,
        "Keyframe exists with time = " +
        std::to_string(
            chr::duration_cast<chr::microseconds>(tp.time_since_epoch()).count())
    };
  }
  _keyframes.push_back(keyframe);
  _keyframesByTime[tp] = keyframe;
  return std::nullopt;
}

bool operator==(const Timeline& a, const Timeline& b) {
  return std::equal(a._keyframes.begin(), a._keyframes.end(), b._keyframes.begin());
}

}