#ifndef INCLUDE_UNION_FIND_HPP
#define INCLUDE_UNION_FIND_HPP

#include <algorithm>
#include <vector>

#include <types.hpp>

namespace procon30 {
namespace util {
class union_find {
private:
  std::vector<int32> _data;

public:
  union_find(size_t size) : _data(size, -1) {}

  bool unite(int32 x, int32 y) {
    x = root(x);
    y = root(y);
    if (x != y) {
      if (_data[y] < _data[x]) std::swap(x, y);
      _data[x] += _data[y];
      _data[y] = x;
    }
    return x != y;
  }

  bool same(int32 x, int32 y) { return root(x) == root(y); }

  int32 root(int32 x) { return _data[x] < 0 ? x : _data[x] = root(_data[x]); }

  size_t size(int32 x) { return -_data[root(x)]; }
};
}  // namespace util
}  // namespace procon30

#endif
