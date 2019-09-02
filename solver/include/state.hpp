#ifndef INCLUDE_STATE_HPP
#define INCLUDE_STATE_HPP

#include <array>
#include <tuple>
#include <vector>

#include <boost/unordered_set.hpp>

#include <util/action_unpack.hpp>
#include <util/next_actions.hpp>

#include <agent.hpp>
#include <color.hpp>
#include <field.hpp>
#include <types.hpp>

namespace procon30 {

/*
template <size_t _agent_count>
class base_state {
private:
  size_t _turn, _max_turn;
  field _field;
  std::vector<agent> _allies, _enemies;

public:
  base_state(size_t turn, size_t max_turn, const field& field, const std::vector<agent>& allies,
             const std::vector<agent>& enemies)
      : _turn(turn), _max_turn(max_turn), _field(field), _allies(allies), _enemies(enemies) {}

  size_t turn() const { return _turn; }
  size_t max_turn() const { return _max_turn; }
  size_t agent_count() const { return _agent_count; }

  const field& field() const { return _field; }

  const std::vector<agent>& allies() const { return _allies; }

  const std::vector<agent>& enemies() const { return _enemies; }

  // virtual auto next_states() const;

  virtual int32 tile_point(color c) const = 0;
  virtual int32 area_point(color c) const = 0;
};
*/

template <size_t _agent_count, size_t _hash_length>
class normal_state {
private:
  size_t _turn, _max_turn;
  field _field;
  std::vector<agent> _allies, _enemies;

  field_state_hash<_hash_length> _ally_hash, _enemy_hash;

  bool can_use_state(const normal_state& state) const {
    boost::unordered_set<std::tuple<size_t, size_t>> check_hash;
    const size_t field_height = state.field().height(), field_width = state.field().width();

    const auto& ally_hash = state.ally_hash();
    const auto& enemy_hash = state.enemy_hash();

    for (size_t i = 0; i < state.agent_count(); ++i) {
      const auto& ally = state.allies().at(i);

      // エージェントが場外にいないか?
      if (!(0 <= ally.y() && ally.y() < field_height && 0 <= ally.x() && ally.x() < field_width)) break;

      // 相手チームマスとかぶってないか
      if (enemy_hash.test(field_width * ally.y() + ally.x())) break;

      check_hash.insert(std::make_tuple(ally.y(), ally.x()));
    }

    for (size_t i = 0; i < state.agent_count(); ++i) {
      const auto& enemy = state.enemies().at(i);

      // エージェントが場外にいないか?
      if (!(0 <= enemy.y() && enemy.y() < field_height && 0 <= enemy.x() && enemy.x() < field_width)) break;

      // 自チームマスとかぶってないか
      if (ally_hash.test(field_width * enemy.y() + enemy.x())) break;

      check_hash.insert(std::make_tuple(enemy.y(), enemy.x()));
    }

    return check_hash.size() == state.agent_count() * 2;
  }

public:
  normal_state(size_t turn, size_t max_turn, const field& field, const std::vector<agent>& allies,
             const std::vector<agent>& enemies)
      : _turn(turn), _max_turn(max_turn), _field(field), _allies(allies), _enemies(enemies), _ally_hash(), _enemy_hash() {}

  normal_state(size_t turn, size_t max_turn, const procon30::field& field, const std::vector<agent>& allies,
               const std::vector<agent>& enemies, const field_state_hash<_hash_length>& ally_hash,
               const field_state_hash<_hash_length>& enemy_hash)
      : _turn(turn), _max_turn(max_turn), _field(field), _allies(allies), _enemies(enemies), _ally_hash(ally_hash), _enemy_hash(enemy_hash) {}

  size_t turn() const { return _turn; }
  size_t max_turn() const { return _max_turn; }
  size_t agent_count() const { return _agent_count; }

  const field& field() const { return _field; }

  const std::vector<agent>& allies() const { return _allies; }

  const std::vector<agent>& enemies() const { return _enemies; }

  auto next_states() const {
    static constexpr std::array<int32, 9> dy = {0, 0, 1, 1, 1, 0, -1, -1, -1}, dx = {0, 1, 1, 0, -1, -1, -1, 0, 1};
    std::vector<normal_state> ret;

    if (this->turn() >= this->max_turn()) return ret;

    // パターンの列挙
    constexpr auto pattern = util::next_actions<_agent_count>();
    for (size_t i = 0; i < pattern.size(); ++i) {
      auto&& action = pattern.at(i);
      auto&& unpack = util::action_unpack<_agent_count>(action);
      std::vector<agent> next_allies = this->allies();
      std::vector<agent> next_enemies = this->enemies();
      field_state_hash<_hash_length> next_ally_hash = this->ally_hash();
      field_state_hash<_hash_length> next_enemy_hash = this->enemy_hash();
      const auto& field = this->field();

      for (size_t i = 0; i < _agent_count * 2; ++i) {
        size_t ny = dy.at(unpack.at(i)), nx = dx.at(unpack.at(i));
        if (i < _agent_count) {  // 自チームの列挙
          agent& next_agent = next_allies.at(i);

          // 移動
          next_agent.move(ny, nx);

          // フィールド範囲チェック
          if (!(0 <= next_agent.y() && next_agent.y() < field.height() && 0 <= next_agent.x() &&
                next_agent.x() < field.width()))
            continue;

          // 移動先が相手マスであれば戻って相手マスを削除する行動とする
          if (this->enemy_hash().test(field.width() * next_agent.y() + next_agent.x())) {
            next_enemy_hash.reset(field.width() * next_agent.y() + next_agent.x());
            next_agent.move(-ny, -nx);
          } else {
            next_ally_hash.set(field.width() * next_agent.y() + next_agent.x());
          }
        } else {  // 相手チームの列挙
          agent& next_agent = next_enemies.at(i - _agent_count);

          // 移動
          next_agent.move(ny, nx);

          // フィールド範囲チェック
          if (!(0 <= next_agent.y() && next_agent.y() < field.height() && 0 <= next_agent.x() &&
                next_agent.x() < field.width()))
            continue;

          // 移動先が相手マスであれば戻って相手マスを削除する行動とする
          if (this->ally_hash().test(field.width() * next_agent.y() + next_agent.x())) {
            next_ally_hash.reset(field.width() * next_agent.y() + next_agent.x());
            next_agent.move(-ny, -nx);
          } else {
            next_enemy_hash.set(field.width() * next_agent.y() + next_agent.x());
          }
        }
      }

      auto&& next_state = normal_state(this->turn() + 1, this->max_turn(), field, next_allies, next_enemies,
                                       next_ally_hash, next_enemy_hash);

      if (can_use_state(next_state)) {
        ret.emplace_back(next_state);
      }
    }

    return ret;
  }

  virtual int32 tile_point(color c) const {
    int32 ret = 0;
    const auto& field = this->field();
    for (size_t i = 0; i < _hash_length; ++i) {
      const size_t y = i / field.width(), x = i % field.width();
      const auto& board = c == color::ally ? this->ally_hash() : this->enemy_hash();

      if (board.test(i)) ret += field.at(y, x);
    }

    return ret;
  }

  virtual int32 area_point(color c) const { return 0; }

  const field_state_hash<_hash_length>& ally_hash() const { return _ally_hash; }
  const field_state_hash<_hash_length>& enemy_hash() const { return _enemy_hash; }
  field_state_hash<_hash_length>& ally_hash() { return _ally_hash; }
  field_state_hash<_hash_length>& enemy_hash() { return _enemy_hash; }
};
}  // namespace procon30

#endif
