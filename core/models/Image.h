#pragma once

#include <chrono>
#include <filesystem>
#include <string>

#include <opencv2/core/mat.hpp>

namespace tlp {
namespace fs = std::filesystem;
namespace chr = std::chrono;
using TimePoint = chr::system_clock::time_point;

struct ImageMetadata {
  size_t width;
  size_t height;
  size_t bitDepth;
  size_t nChannel;

  TimePoint timestamp;
  int exposureUs;
  int iso;
  double fStop;

  std::string toJson() const;
  static ImageMetadata fromJson(const std::string& json);
};

class Image {
public:
  std::string toJson() const;
  static Image fromJson(const std::string& json);

private:
  fs::path _filepath;
  // Homography transformation matrix for aligning images.
  cv::Mat _alignHomo;
  // Image metadata extracted from image EXIF data or file header.
  ImageMetadata _metadata;
};

}