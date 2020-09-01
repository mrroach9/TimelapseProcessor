#include <models/Image.h>
#include <models/TypeTraits.h>
#include <models/Utils.h>

#include <chrono>

#include <gtest/gtest.h>
#include <opencv2/core/mat.hpp>

#include <iostream>

using namespace tlp;

TEST(ImageTest, toJson) {
  ImageMetadata meta;
  meta.width = 640;
  meta.height = 480;
  meta.bitDepth = 8;
  meta.nChannel = 3;
  meta.timestamp = TimePoint(chr::microseconds(1234));
  meta.exposureUs = 5678;
  meta.iso = 100;
  meta.fStop = 5.6;

  cv::Mat homo = cv::Mat::eye(3, 3, CV_32F);
  Image image("C:\\Test Path\\test image.jpg", homo, meta);
  rapidjson::Document d;
  EXPECT_EQ(toString(image.toJson(d.GetAllocator()), true),
R"Delim({
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
}
