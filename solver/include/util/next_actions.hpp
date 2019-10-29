#ifndef INCLUDE_NEXT_ACTIONS_HPP
#define INCLUDE_NEXT_ACTIONS_HPP

#include <array>
#include <cstdint>

#include <boost/hana/power.hpp>

#include <types.hpp>

namespace procon30 {
/*
 * 行動パターンを列挙する(コンパイル時計算対応)
 */

namespace util {
namespace hana = boost::hana;
template <std::size_t N>
struct next_actions {
  std::array<action_pattern_type, hana::power(9, hana::int_c<N * 2>)> _array;
  constexpr next_actions() : _array() {}
  template <std::size_t M>
  constexpr auto get() const {
    return _array[M];
  }

  constexpr auto at(std::size_t idx) const { return _array[idx]; }
  constexpr auto size() const { return hana::power(9, hana::int_c<N * 2>); }

  auto begin() { return _array.begin(); };
  auto end() { return _array.end(); };
  auto cbegin() { return _array.cbegin(); };
  auto cend() { return _array.cend(); };
  auto rbegin() { return _array.rbegin(); }
  auto crbegin() { return _array.crbegin(); }
  auto rend() { return _array.rend(); }
  auto crend() { return _array.crend(); }
};

template <>
constexpr next_actions<2>::next_actions() : _array() {
  std::size_t idx = 0;
  for (std::size_t i = 1; i < 9; ++i)
    for (std::size_t j = 1; j < 9; ++j)
      for (std::size_t k = 1; k < 9; ++k)
        for (std::size_t l = 1; l < 9; ++l) {
          _array[idx++] = std::bitset<64>(i | (j << 4) | (k << 8) | (l << 12));
        }
}

// TODO: 3はstd::arrayのメモリが足りないのか動かないのでどうにかする
template <>
constexpr next_actions<3>::next_actions() : _array() {
  std::size_t idx = 0;
  for (std::size_t i = 1; i < 9; ++i)
    for (std::size_t j = 1; j < 9; ++j)
      for (std::size_t k = 1; k < 9; ++k)
        for (std::size_t l = 1; l < 9; ++l)
          for (std::size_t m = 1; m < 9; ++m)
            for (std::size_t n = 1; n < 9; ++n) {
              _array[idx++] = std::bitset<64>(i | (j << 4) | (k << 8) | (l << 12) | (m << 16) | (n << 20));
            }
}
// TODO: 4以上は候補数が多すぎるのでランダム生成するようにする
}  // namespace util
}  // namespace procon30

#endif
