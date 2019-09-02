#ifndef INCLUDE_TYPE_HPP
#define INCLUDE_TYPE_HPP

#include <bitset>
#include <cstdint>

namespace procon30 {
using int8 = std::int_fast8_t;
using int16 = std::int_fast16_t;
using int32 = std::int_fast32_t;
using int64 = std::int_fast64_t;
using uint8 = std::uint_fast8_t;
using uint16 = std::uint_fast16_t;
using uint32 = std::uint_fast32_t;
using uint64 = std::uint_fast64_t;

using eval_score_type = double;
using score_type = int32;

template <size_t N>
using field_state_hash = std::bitset<N>;

using action_pattern_type = std::bitset<64>;
}  // namespace procon30

#endif
