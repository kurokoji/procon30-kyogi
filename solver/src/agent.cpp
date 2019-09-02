#include <agent.hpp>

namespace procon30 {
agent::agent() {}
agent::agent(size_t y, size_t x, size_t agent_id, color team_color)
    : _y(y), _x(x), _agent_id(agent_id), _team_color(team_color) {}

size_t agent::y() const {
  return _y;
}

size_t agent::x() const {
  return _x;
}

void agent::move(int32 y, int32 x) {
  using namespace std::literals;
  if (!(-1 <= y && y <= 1 && -1 <= x && x <= 1)) {
    throw std::out_of_range("out_of_range: agent::move");
  }

  _y += y;
  _x += x;
}
}  // namespace procon30
