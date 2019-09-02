#ifndef INCLUDE_STATE_VISUALIZE_HPP
#define INCLUDE_STATE_VISUALIZE_HPP

#include <iostream>
#include <sstream>

#include <boost/format.hpp>

#include <state.hpp>
#include <types.hpp>
#include <field.hpp>

namespace procon30 {
namespace util {
template <typename state_type>
void state_visualize(state_type state) {
  const auto& field = state.field();
  const auto height = field.height(), width = field.width();

  std::stringstream ret;

  ret << " ";
  for (size_t i = 0; i < width; ++i) {
    ret << boost::format("%4d") % i;
  }
  ret << "\n";
  ret << "  ┌";

  for (size_t i = 0; i < width; ++i) {
    ret << boost::format("───%s") % (i != width - 1 ? "┬" : "┐");
  }
  ret << "\n";

  for (size_t i = 0; i < height; ++i) {
    ret << boost::format("%2s│") % i;
    for (size_t j = 0; j < width; ++j) {
      bool is_blank = !(state.ally_hash() | state.enemy_hash()).test(width * i + j);
      bool is_ally = state.ally_hash().test(width * i + j);
      bool is_enemy = state.enemy_hash().test(width * i + j);
      int32 score = field.at(i, j);

      uint32 text_attribute, background_color;

      text_attribute = is_blank ? 0 : 4;

      if (is_blank) {
        background_color = 40;
      } else if (is_ally) {
        background_color = 44;
      } else if (is_enemy) {
        background_color = 41;
      }

      ret << boost::format("\x1b[%d;%sm%3d\x1b[m%s") % text_attribute % background_color % score % "│";
    }

    ret << "\n";

    if (i != height - 1) {
      ret << "  ├";
      for (size_t j = 0; j < width; ++j) {
        ret << boost::format("───%s") % (j != width - 1 ? "┼" : "┤");
      }
    } else {
      ret << "  └";
      for (size_t j = 0; j < width; ++j) {
        ret << boost::format("───%s") % (j != width - 1 ? "┴" : "┘");
      }
    }

    ret << "\n";
  }

  std::cerr << ret.str() << std::endl;
}
}  // namespace util
}  // namespace procon30

#endif
