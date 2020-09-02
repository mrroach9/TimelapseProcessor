#pragma once

#include <models/Image.h>
#include <models/Timeline.h>
#include <models/TypeTraits.h>

#include <filesystem>
#include <map>
#include <set>

namespace tlp {

enum class OutputFormat {
  H264_1080P,
  SEQ_OF_IMAGES,
};

class Project {
public:
  std::string toJson() const;
  static Project fromJson(const std::string& json);

private:
  std::map<size_t, Image> _imagesById;
  Timeline _timeline;
  std::string _projectName;
  OutputFormat _outputFormat;
};

}