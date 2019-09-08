#ifndef INCLUDE_SINGLE_NEXT_ACTIONS_HPP
#define INCLUDE_SINGLE_NEXT_ACTIONS_HPP

#include <array>
#include <cstdint>

#include <boost/hana/power.hpp>

#include <types.hpp>

namespace procon30 {
namespace util {
namespace hana = boost::hana;
template <std::size_t N>
struct single_next_actions {
  std::array<action_pattern_type, hana::power(9, hana::int_c<N>)> _array;
  constexpr single_next_actions() : _array() {}
  template <std::size_t M>
  constexpr auto get() const {
    return _array[M];
  }

  constexpr auto at(std::size_t idx) const { return _array[idx]; }
  constexpr auto size() const { return hana::power(9, hana::int_c<N>); }

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
constexpr single_next_actions<2>::single_next_actions() : _array() {
  std::size_t idx = 0;
  for (std::size_t i = 0; i < 9; ++i)
    for (std::size_t j = 0; j < 9; ++j) {
      _array[idx++] = std::bitset<64>(i | (j << 4));
    }
}

template <>
constexpr single_next_actions<3>::single_next_actions() : _array() {
  std::size_t idx = 0;
  for (std::size_t i = 0; i < 9; ++i)
    for (std::size_t j = 0; j < 9; ++j)
      for (std::size_t k = 0; k < 9; ++k) {
        _array[idx++] = std::bitset<64>(i | (j << 4) | (k << 8));
      }
}

template <>
constexpr single_next_actions<4>::single_next_actions() : _array() {
  std::size_t idx = 0;
  for (std::size_t i = 0; i < 9; ++i)
    for (std::size_t j = 0; j < 9; ++j)
      for (std::size_t k = 0; k < 9; ++k)
        for (std::size_t l = 0; l < 9; ++l) {
          _array[idx++] = std::bitset<64>(i | (j << 4) | (k << 8) | (l << 12));
        }
}

template <>
constexpr single_next_actions<5>::single_next_actions() : _array() {
  std::size_t idx = 0;
  for (std::size_t i = 0; i < 9; ++i)
    for (std::size_t j = 0; j < 9; ++j)
      for (std::size_t k = 0; k < 9; ++k)
        for (std::size_t l = 0; l < 9; ++l)
          for (std::size_t m = 0; m < 9; ++m) {
            _array[idx++] = std::bitset<64>(i | (j << 4) | (k << 8) | (l << 12) | (m << 16));
          }
}

}  // namespace util
}  // namespace procon30

#endif
