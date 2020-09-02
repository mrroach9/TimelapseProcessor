#pragma once

#include <models/TypeTraits.h>

#include <chrono>
#include <filesystem>
#include <string>

#include <opencv2/core/mat.hpp>
#include <rapidjson/document.h>

namespace tlp {

struct ImageMetadata {
  size_t width;
  size_t height;
  size_t bitDepth;
  size_t nChannel;

  TimePoint timestamp;
  int exposureUs;
  int iso;
  double fStop;

  rapidjson::Value toJson(JsonAlloc& allocator) const;
  static ImageMetadata fromJson(const rapidjson::Value& json);
};

class Image {
public:
  // TODO: Delete these explicit constructors when parsing feature is added. Temporarily
  // added for testing purpose.
  Image(size_t imageId, const fs::path& path, const cv::Mat& alignHomo,
      const ImageMetadata& metadata):
      _imageId(imageId), _filepath(path), _alignHomo(alignHomo), _metadata(metadata) {}
  Image() {}

  rapidjson::Value toJson(JsonAlloc& allocator) const;
  static Image fromJson(const rapidjson::Value& json);

  fs::path filepath() const;

private:
  size_t _imageId;
  fs::path _filepath;
  // Homography transformation matrix for aligning images.
  cv::Mat _alignHomo;
  // Image metadata extracted from image EXIF data or file header.
  ImageMetadata _metadata;
};

}