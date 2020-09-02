#include <models/Timeline.h>
#include <models/TypeTraits.h>
#include <models/Utils.h>

#include <gtest/gtest.h>
#include <opencv2/core/types.hpp>

using namespace tlp;

TEST(TimelineTest, toJson) {
  Keyframe k1(1, cv::Rect2d(1, 2, 3, 4), 0.3, InterpMethod::NO_INTERP);
  Keyframe k2(123, cv::Rect2d(-2, -3, 100, 2000), -0.5, InterpMethod::CUBIC);
  Timeline timeline({
    {TimePoint(chr::microseconds(0)), k1},
    {TimePoint(chr::microseconds(100)), k2}
  });
  rapidjson::Document d;
  EXPECT_EQ(toString(timeline.toJson(d.GetAllocator()), true),
R"Delim({
    "keyframes": [
        {
            "ref_image_id": 1,
            "crop_rect": [
                1.0,
                2.0,
                3.0,
                4.0
            ],
            "ev_delta": 0.3,
            "interp_method": "NO_INTERP"
        },
        {
            "ref_image_id": 123,
            "crop_rect": [
                -2.0,
                -3.0,
                100.0,
                2000.0
            ],
            "ev_delta": -0.5,
            "interp_method": "CUBIC"
        }
    ]
})Delim");
}
