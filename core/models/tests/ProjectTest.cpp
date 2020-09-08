#include <models/Image.h>
#include <models/Project.h>
#include <models/Timeline.h>
#include <models/TypeTraits.h>
#include <models/Utils.h>

#include <gtest/gtest.h>

using namespace tlp;

TEST(ProjectTest, toJson) {
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
  Image image1(0, "C:\\Test Path\\image1.jpg", homo, meta);
  Image image2(1, "C:\\Test Path\\image2.jpg", homo, meta);
  
  Keyframe k1{0, cv::Rect2d(1, 2, 3, 4), 0.0, InterpMethod::NO_INTERP};
  Keyframe k2{1, cv::Rect2d(1, 2, 3, 4), 0.0, InterpMethod::CUBIC};
  Timeline timeline({k1, k2});

  Project project(
      "Project",
      VideoEncoding::H264,
      VideoResolution::RES_1080P,
      {image1, image2},
      timeline);
  
  rapidjson::Document d;
  rapidjson::Value json = project.toJson(d.GetAllocator()); 
  EXPECT_EQ(toString(json, true),
R"Delim({
    "project_name": "Project",
    "export_encoding": "H264",
    "export_resolution": "1080p",
    "timeline": {
        "keyframes": [
            {
                "ref_image_id": 0,
                "crop_rect": [
                    1.0,
                    2.0,
                    3.0,
                    4.0
                ],
                "ev_delta": 0.0,
                "interp_method": "NO_INTERP"
            },
            {
                "ref_image_id": 1,
                "crop_rect": [
                    1.0,
                    2.0,
                    3.0,
                    4.0
                ],
                "ev_delta": 0.0,
                "interp_method": "CUBIC"
            }
        ]
    },
    "images": [
        {
            "id": 0,
            "filepath": "C:\\Test Path\\image1.jpg",
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
        },
        {
            "id": 1,
            "filepath": "C:\\Test Path\\image2.jpg",
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
        }
    ]
})Delim");

  const auto maybeOut = Project::fromJson(json);
  EXPECT_EQ(maybeOut, project);
}
