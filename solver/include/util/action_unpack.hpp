#ifndef INCLUDE_ACTION_UNPACK
#define INCLUDE_ACTION_UNPACK

#include <array>

#include <types.hpp>

namespace procon30 {
namespace util {

template <std::size_t N>
auto action_unpack(const action_pattern_type& action) {
  std::array<std::size_t, N * 2> ret;
  action_pattern_type mask("1111");

  for (std::size_t i = 0; i < N * 2; ++i) {
    auto v = (action >> (i * 4)) & mask;
    ret.at(i) = ((action >> (i * 4)) & mask).to_ulong();
  }

  return ret;
}
}  // namespace util
}  // namespace procon30

#endif
