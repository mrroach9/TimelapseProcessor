#include <models/Image.h>

#include <common/Math.h>
#include <common/Utils.h>
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
  const auto maybeWidth = getValueFromJsonChild<size_t>(json, "width");
  const auto maybeHeight = getValueFromJsonChild<size_t>(json, "height");
  const auto maybeBitDepth = getValueFromJsonChild<size_t>(json, "bit_depth");
  const auto maybeNChannel = getValueFromJsonChild<size_t>(json, "n_channel");
  const auto maybeExposureUs = getValueFromJsonChild<size_t>(json, "exposure_us");
  const auto maybeIso = getValueFromJsonChild<size_t>(json, "iso");
  const auto maybeTimestamp = getValueFromJsonChild<uint64_t>(json, "timestamp_us");
  const auto maybeFStop = getValueFromJsonChild<double>(json, "f_stop");

  if (const auto maybeError = aggregateErrors(
      maybeWidth, maybeHeight, maybeBitDepth, maybeNChannel,
      maybeExposureUs, maybeIso, maybeTimestamp, maybeFStop)) {
    return tl::unexpected(*maybeError);
  }

  meta.width = maybeWidth.value();
  meta.height = maybeHeight.value();
  meta.nChannel = maybeNChannel.value();
  meta.bitDepth = maybeBitDepth.value();
  meta.exposureUs = maybeExposureUs.value();
  meta.iso = maybeIso.value();
  meta.timestamp = TimePoint(chr::microseconds(maybeTimestamp.value()));
  meta.fStop = maybeFStop.value();
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

  const auto maybeId = getValueFromJsonChild<size_t>(json, "id");
  const auto maybeFilepath = getValueFromJsonChild<const char*>(json, "filepath");

  tl::expected<cv::Mat, Error> maybeAlignHomo;
  if (json.HasMember("align_homo")) {
    maybeAlignHomo = mat3dFromJson(json["align_homo"]);
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "align_homo field is missing!"
    });
  }

  tl::expected<ImageMetadata, Error> maybeMeta;
  if (json.HasMember("metadata")) {
    maybeMeta = ImageMetadata::fromJson(json["metadata"]);
  } else {
    return tl::unexpected(Error{
          ErrorCode::JSON_MISSING_FIELD, "metadata field is missing!"
    });
  }

  if (const auto maybeError = aggregateErrors(
      maybeId, maybeFilepath, maybeAlignHomo, maybeMeta)) {
    return tl::unexpected(*maybeError);
  }
  image._imageId = maybeId.value();
  image._filepath = maybeFilepath.value();
  image._alignHomo = maybeAlignHomo.value();
  image._metadata = maybeMeta.value();

  return image;
}

std::string Image::filepath() const {
  return _filepath;
}

size_t Image::id() const {
  return _imageId;
}

ImageMetadata Image::metadata() const {
  return _metadata;
}

bool operator==(const Image& a, const Image &b) {
  return a._imageId == b._imageId
      && a._filepath == b._filepath
      && std::equal(a._alignHomo.begin<double>(), a._alignHomo.end<double>(),
                    b._alignHomo.begin<double>(), double_eq)
      && a._metadata == b._metadata;
}

}