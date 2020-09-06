#include <models/Image.h>

#include <common/Math.h>
#include <models/Utils.h>

#include <chrono>
#include <iostream>

namespace tlp {

rapidjson::Value ImageMetadata::toJson(JsonAlloc& allocator) const {
  rapidjson::Value val;
  val.SetObject()
      .AddMember("width", width, allocator)
      .AddMember("height", height, allocator)
      .AddMember("bit_depth", bitDepth, allocator)
      .AddMember("n_channel", nChannel, allocator)
      .AddMember("timestamp_us",
          chr::duration_cast<chr::microseconds>(timestamp.time_since_epoch()).count(),
          allocator)
      .AddMember("exposure_us", exposureUs, allocator)
      .AddMember("iso", iso, allocator)
      .AddMember("f_stop", fStop, allocator);
  return val;
}

tl::expected<ImageMetadata, Error> ImageMetadata::fromJson(const rapidjson::Value& json) {
  ImageMetadata meta;
  if (!json.IsObject()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "ImageMetadata JSON node is not an object!"});
  }
  const auto maybeWidth = getUintFromJsonChild(json, "width");
  if (!maybeWidth) {
    return tl::unexpected(maybeWidth.error());
  } else {
    meta.width = maybeWidth.value();
  }

  const auto maybeHeight = getUintFromJsonChild(json, "height");
  if (!maybeHeight) {
    return tl::unexpected(maybeHeight.error());
  } else {
    meta.height = maybeHeight.value();
  }

  const auto maybeBitDepth = getUintFromJsonChild(json, "bit_depth");
  if (!maybeBitDepth) {
    return tl::unexpected(maybeBitDepth.error());
  } else {
    meta.bitDepth = maybeBitDepth.value();
  }

  const auto maybeNChannel = getUintFromJsonChild(json, "n_channel");
  if (!maybeNChannel) {
    return tl::unexpected(maybeNChannel.error());
  } else {
    meta.nChannel = maybeNChannel.value();
  }

  const auto maybeExposureUs = getUintFromJsonChild(json, "exposure_us");
  if (!maybeExposureUs) {
    return tl::unexpected(maybeExposureUs.error());
  } else {
    meta.exposureUs = maybeExposureUs.value();
  }

  const auto maybeIso = getUintFromJsonChild(json, "iso");
  if (!maybeIso) {
    return tl::unexpected(maybeIso.error());
  } else {
    meta.iso = maybeIso.value();
  }

  if (json.HasMember("timestamp_us")) {
    const auto& timestampJson = json["timestamp_us"];
    if (!timestampJson.IsUint64()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE, "timestamp_us is not a uint64!"});
    }
    meta.timestamp = TimePoint(chr::microseconds(timestampJson.GetUint64()));
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "timestamp_us field is missing!"
    });
  }

  if (json.HasMember("f_stop")) {
    const auto& fStopJson = json["f_stop"];
    if (!fStopJson.IsDouble()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE, "f_stop is not a uint64!"});
    }
    meta.fStop = fStopJson.GetDouble();
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "f-stop field is missing!"
    });
  }
  return meta;
}

bool operator==(const ImageMetadata& a, const ImageMetadata& b) {
  return a.width == b.width
      && a.height == b.height
      && a.bitDepth == b.bitDepth
      && a.nChannel == b.nChannel
      && a.timestamp == b.timestamp
      && a.exposureUs == b.exposureUs
      && a.iso == b.iso
      && double_eq(a.fStop, b.fStop);
}

rapidjson::Value Image::toJson(JsonAlloc& allocator) const {
  rapidjson::Value val;
  val.SetObject()
      .AddMember("id", _imageId, allocator)
      .AddMember("filepath", toStringRef(_filepath), allocator)
      .AddMember("align_homo", mat3dToJson(_alignHomo, allocator), allocator)
      .AddMember("metadata", _metadata.toJson(allocator), allocator);
  return val;
}

tl::expected<Image, Error> Image::fromJson(const rapidjson::Value& json) {
  Image image;
  if (!json.IsObject()) {
    return tl::unexpected(Error{
        ErrorCode::JSON_WRONG_NODE_TYPE, "Image JSON node is not an object!"});
  }

  const auto maybeId = getUintFromJsonChild(json, "id");
  if (!maybeId) {
    return tl::unexpected(maybeId.error());
  } else {
    image._imageId = maybeId.value();
  }

  if (json.HasMember("filepath")) {
    const auto& filepathJson = json["filepath"];
    if (!filepathJson.IsString()) {
      return tl::unexpected(Error{
          ErrorCode::JSON_WRONG_NODE_TYPE, "filepath is not a string!"});
    }
    image._filepath = filepathJson.GetString();
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "filepath field is missing!"
    });
  }

  if (json.HasMember("align_homo")) {
    const auto maybeAlignHomo = mat3dFromJson(json["align_homo"]);
    if (maybeAlignHomo) {
      image._alignHomo = maybeAlignHomo.value();
    } else {
      return tl::unexpected(maybeAlignHomo.error());
    }
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "align_homo field is missing!"
    });
  }

  if (json.HasMember("metadata")) {
    const auto maybeMeta = ImageMetadata::fromJson(json["metadata"]);
    if (maybeMeta) {
      image._metadata = maybeMeta.value();
    } else {
      return tl::unexpected(maybeMeta.error());
    }
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "metadata field is missing!"
    });
  }

  return image;
}

std::string Image::filepath() const {
  return _filepath;
}

size_t Image::id() const {
  return _imageId;
}

bool operator==(const Image& a, const Image &b) {
  return a._imageId == b._imageId
      && a._filepath == b._filepath
      && std::equal(a._alignHomo.begin<double>(), a._alignHomo.end<double>(),
                    b._alignHomo.begin<double>(), double_eq)
      && a._metadata == b._metadata;
}

}