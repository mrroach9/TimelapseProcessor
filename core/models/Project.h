#pragma once

#include <models/TypeTraits.h>

#include <filesystem>
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
  std::set<fs::path> _inputImages;
  std::string _projectName;
  OutputFormat _outputFormat;
};

}