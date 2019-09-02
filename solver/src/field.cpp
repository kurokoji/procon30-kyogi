#include <stdexcept>
#include <vector>

#include <field.hpp>
#include <types.hpp>

namespace procon30 {
field::field() {}
field::field(size_t height, size_t width, const std::vector<std::vector<int32>>& score)
    : _height(height), _width(width), _score(score) {}

int32 field::at(size_t height, size_t width) const {
  using namespace std::literals;
  if (!(0 <= height && height < _height && 0 <= width && width < _width)) {
    throw std::out_of_range("out of range: field.at"s);
  }
  return _score.at(height).at(width);
}

size_t field::height() const {
  return _height;
}

size_t field::width() const {
  return _width;
}
}  // namespace procon30
