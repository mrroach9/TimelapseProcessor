#include <models/Image.h>

#include <models/Utils.h>

#include <chrono>

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

ImageMetadata ImageMetadata::fromJson(const rapidjson::Value& json) {
  // TODO: implement this.
  return ImageMetadata{};
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

Image Image::fromJson(const rapidjson::Value& json) {
  // TODO: implement this.
  return Image();
}

fs::path Image::filepath() const {
  return _filepath;
}

}