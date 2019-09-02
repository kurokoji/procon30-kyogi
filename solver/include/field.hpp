#ifndef INCLUDE_FIELD_HPP
#define INCLUDE_FIELD_HPP

#include <stdexcept>
#include <vector>

#include <types.hpp>

namespace procon30 {
/*
 * フィールド情報
 */
class field {
private:
  size_t _height, _width;
  std::vector<std::vector<int32>> _score;

public:
  field();
  field(size_t height, size_t width, const std::vector<std::vector<int32>>& score);
  /*
   * at(3, 3) -> 3行3列目の要素にアクセス(const)
   */
  int32 at(size_t height, size_t width) const;
  size_t height() const;
  size_t width() const;
};
}  // namespace procon30

#endif
