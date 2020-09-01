#pragma once

#include <chrono>
#include <filesystem>

#include <rapidjson/document.h>

namespace tlp {

namespace fs = std::filesystem;
namespace chr = std::chrono;

using TimePoint = chr::system_clock::time_point;
using JsonAlloc = rapidjson::Document::AllocatorType;

}