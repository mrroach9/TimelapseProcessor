#include <models/Image.h>

namespace tlp {

std::string ImageMetadata::toJson() const {
  // TODO: implement this.
  return "ImageMetadata";
}

ImageMetadata ImageMetadata::fromJson(const std::string& json) {
  // TODO: implement this.
  return ImageMetadata{};
}

std::string Image::toJson() const {
  // TODO: implement this.
  return "Image";
}

Image Image::fromJson(const std::string& json) {
  // TODO: implement this.
  return Image();
}

}