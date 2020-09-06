#include <models/Image.h>
#include <models/TypeTraits.h>
#include <models/Utils.h>

#include <chrono>

#include <gtest/gtest.h>
#include <opencv2/core/mat.hpp>

using namespace tlp;

TEST(ImageTest, TwoWayJsonConversion) {
  ImageMetadata meta;
  meta.width = 640;
  meta.height = 480;
  meta.bitDepth = 8;
  meta.nChannel = 3;
  meta.timestamp = TimePoint(chr::microseconds(1234));
  meta.exposureUs = 5678;
  meta.iso = 100;
  meta.fStop = 5.6;

  cv::Mat homo = cv::Mat::eye(3, 3, CV_64F);
  Image image(123, "C:\\Test Path\\test image.jpg", homo, meta);
  rapidjson::Document d;
  rapidjson::Value json = image.toJson(d.GetAllocator());
  EXPECT_EQ(toString(json, true),
R"Delim({
    "id": 123,
    "filepath": "C:\\Test Path\\test image.jpg",
    "align_homo": [
        [
            1.0,
            0.0,
            0.0
        ],
        [
            0.0,
            1.0,
            0.0
        ],
        [
            0.0,
            0.0,
            1.0
        ]
    ],
    "metadata": {
        "width": 640,
        "height": 480,
        "bit_depth": 8,
        "n_channel": 3,
        "timestamp_us": 1234,
        "exposure_us": 5678,
        "iso": 100,
        "f_stop": 5.6
    }
})Delim");

  const auto maybeOut = Image::fromJson(json);
  EXPECT_EQ(maybeOut.value(), image);
}
