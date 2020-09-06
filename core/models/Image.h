#pragma once

#include <common/Error.h>
#include <models/TypeTraits.h>

#include <chrono>
#include <filesystem>
#include <string>

#include <tl/expected.hpp>
#include <opencv2/core/mat.hpp>
#include <rapidjson/document.h>

namespace tlp {

struct ImageMetadata {
  size_t width;
  size_t height;
  size_t bitDepth;
  size_t nChannel;

  TimePoint timestamp;
  size_t exposureUs;
  size_t iso;
  double fStop;

  rapidjson::Value toJson(JsonAlloc& allocator) const;
  static tl::expected<ImageMetadata, Error> fromJson(const rapidjson::Value& json);
};

class Image {
public:
  // TODO: Delete these explicit constructors when parsing feature is added. Temporarily
  // added for testing purpose.
  Image(size_t imageId, const std::string& path, const cv::Mat& alignHomo,
      const ImageMetadata& metadata):
      _imageId(imageId), _filepath(path), _alignHomo(alignHomo), _metadata(metadata) {}
  Image() {}

  rapidjson::Value toJson(JsonAlloc& allocator) const;
  static tl::expected<Image, Error> fromJson(const rapidjson::Value& json);

  std::string filepath() const;
  size_t id() const;

private:
  size_t _imageId;
  // UTF-8 encoded bytestreams of filepaths. (Currently only supporting ASCII-chars).
  std::string _filepath;
  // Homography transformation matrix for aligning images.
  cv::Mat _alignHomo;
  // Image metadata extracted from image EXIF data or file header.
  ImageMetadata _metadata;
};

}