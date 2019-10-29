#ifndef INCLUDE_SINGLE_ACTION_UNPACK
#define INCLUDE_SINGLE_ACTION_UNPACK

#include <array>

#include <types.hpp>

namespace procon30 {
namespace util {
template <std::size_t N>
auto single_action_unpack(const action_pattern_type& action) {
  std::array<std::size_t, N> ret;
  action_pattern_type mask("1111");

  for (std::size_t i = 0; i < N; ++i) {
    ret.at(i) = ((action >> (i * 4)) & mask).to_ulong();
  }

  return ret;
}
}  // namespace util
}  // namespace procon30

#endif
