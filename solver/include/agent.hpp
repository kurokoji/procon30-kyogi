#ifndef INCLUDE_AGENT_HPP
#define INCLUDE_AGENT_HPP

#include <color.hpp>
#include <types.hpp>

namespace procon30 {
/*
 * エージェント情報
 */
class agent {
private:
  size_t _y, _x;
  size_t _agent_id;
  color _team_color;

public:
  agent();
  agent(size_t y, size_t x, size_t agent_id, color team_color);
  size_t y() const;
  size_t x() const;
  /*
   * move(-1, 0) -> 上に移動させる
   * move(1, 1) -> 右下に移動させる
   */
  void move(int32 y, int32 x);
};
}  // namespace procon30

#endif
