#ifndef INCLUDE_SINGLE_STATE_HPP
#define INCLUDE_SINGLE_STATE_HPP

#include <array>
#include <iostream>
#include <tuple>
#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <util/single_action_unpack.hpp>
#include <util/single_next_actions.hpp>
#include <util/union_find.hpp>

#include <agent.hpp>
#include <color.hpp>
#include <field.hpp>
#include <types.hpp>

namespace procon30 {
template <size_t _agent_count, size_t _height, size_t _width>
class single_normal_state {
private:
  static constexpr size_t _hash_length = _height * _width;
  size_t _turn, _max_turn;
  field _field;
  std::vector<agent> _allies, _enemies;

  field_state_hash<_hash_length> _ally_hash, _enemy_hash;

  bool can_use_state(const single_normal_state& state) const {
    boost::unordered_set<std::tuple<size_t, size_t>> check_hash;
    const auto& ally_hash = state.ally_hash();
    const auto& enemy_hash = state.enemy_hash();

    for (size_t i = 0; i < state.agent_count(); ++i) {
      const auto& ally = state.allies().at(i);

      // エージェントが場外にいないか?
      if (!(0 <= ally.y() && ally.y() < _height && 0 <= ally.x() && ally.x() < _width)) break;

      // 相手チームマスとかぶってないか
      if (enemy_hash.test(_width * ally.y() + ally.x())) break;

      check_hash.insert(std::make_tuple(ally.y(), ally.x()));
    }

    for (size_t i = 0; i < state.agent_count(); ++i) {
      const auto& enemy = state.enemies().at(i);

      // エージェントが場外にいないか?
      if (!(0 <= enemy.y() && enemy.y() < _height && 0 <= enemy.x() && enemy.x() < _width)) break;

      // 自チームマスとかぶってないか
      if (ally_hash.test(_width * enemy.y() + enemy.x())) break;

      check_hash.insert(std::make_tuple(enemy.y(), enemy.x()));
    }

    return check_hash.size() == state.agent_count() * 2;
  }

public:
  single_normal_state(size_t turn, size_t max_turn, const field& field, const std::vector<agent>& allies,
               const std::vector<agent>& enemies)
      : _turn(turn),
        _max_turn(max_turn),
        _field(field),
        _allies(allies),
        _enemies(enemies),
        _ally_hash(),
        _enemy_hash() {}

  single_normal_state(size_t turn, size_t max_turn, const procon30::field& field, const std::vector<agent>& allies,
               const std::vector<agent>& enemies, const field_state_hash<_hash_length>& ally_hash,
               const field_state_hash<_hash_length>& enemy_hash)
      : _turn(turn),
        _max_turn(max_turn),
        _field(field),
        _allies(allies),
        _enemies(enemies),
        _ally_hash(ally_hash),
        _enemy_hash(enemy_hash) {}

  size_t turn() const { return _turn; }
  size_t max_turn() const { return _max_turn; }
  size_t agent_count() const { return _agent_count; }

  const field& field() const { return _field; }

  const std::vector<agent>& allies() const { return _allies; }

  const std::vector<agent>& enemies() const { return _enemies; }

  auto next_states(color c) const {
    static constexpr std::array<int32, 9> dy = {0, 0, 1, 1, 1, 0, -1, -1, -1}, dx = {0, 1, 1, 0, -1, -1, -1, 0, 1};
    std::vector<single_normal_state> ret;

    if (this->turn() >= this->max_turn()) return ret;

    // パターンの列挙
    static constexpr auto pattern = util::single_next_actions<_agent_count>();
    for (size_t j = 0; j < pattern.size(); ++j) {
      auto&& action = pattern.at(j);
      auto&& unpack = util::single_action_unpack<_agent_count>(action);
      std::vector<agent> next_allies = this->allies();
      std::vector<agent> next_enemies = this->enemies();
      field_state_hash<_hash_length> next_ally_hash = this->ally_hash();
      field_state_hash<_hash_length> next_enemy_hash = this->enemy_hash();
      const auto& field = this->field();

      for (size_t i = 0; i < _agent_count; ++i) {
        //std::cerr << unpack.at(i) << std::endl;
        size_t ny = dy.at(unpack.at(i)), nx = dx.at(unpack.at(i));
        if (c == color::ally) {  // 自チームの列挙
          agent& next_agent = next_allies.at(i);

          // 移動
          next_agent.move(ny, nx);

          // フィールド範囲チェック
          if (!(0 <= next_agent.y() && next_agent.y() < _height && 0 <= next_agent.x() && next_agent.x() < _width))
            continue;

          // 移動先が相手マスであれば戻って相手マスを削除する行動とする
          if (this->enemy_hash().test(_width * next_agent.y() + next_agent.x())) {
            next_enemy_hash.reset(_width * next_agent.y() + next_agent.x());
            next_agent.move(-ny, -nx);
          } else {
            next_ally_hash.set(_width * next_agent.y() + next_agent.x());
          }
        } else {  // 相手チームの列挙
          agent& next_agent = next_enemies.at(i);

          // 移動
          next_agent.move(ny, nx);

          // フィールド範囲チェック
          if (!(0 <= next_agent.y() && next_agent.y() < _height && 0 <= next_agent.x() && next_agent.x() < _width))
            continue;

          // 移動先が相手マスであれば戻って相手マスを削除する行動とする
          if (this->ally_hash().test(_width * next_agent.y() + next_agent.x())) {
            next_ally_hash.reset(_width * next_agent.y() + next_agent.x());
            next_agent.move(-ny, -nx);
          } else {
            next_enemy_hash.set(_width * next_agent.y() + next_agent.x());
          }
        }
      }

      auto&& next_state = single_normal_state(this->turn() + (c == color::enemy ? 1 : 0), this->max_turn(), field, next_allies, next_enemies,
                                       next_ally_hash, next_enemy_hash);

      if (can_use_state(next_state)) {
        ret.emplace_back(next_state);
      }
    }

    return ret;
  }

  int32 tile_point(color c) const {
    int32 ret = 0;
    const auto& field = this->field();
    for (size_t i = 0; i < _hash_length; ++i) {
      const size_t y = i / _width, x = i % _width;
      const auto& board = c == color::ally ? this->ally_hash() : this->enemy_hash();
      if (board.test(i)) ret += field.at(y, x);
    }

    return ret;
  }

  int32 area_point(color c) const {
    static constexpr std::array<int32, 4> dy = {0, 1, 0, -1}, dx = {1, 0, -1, 0};

    int32 ret = 0;
    const auto& board = c == color::ally ? this->ally_hash() : this->enemy_hash();

    auto is_between = [&](size_t y, size_t x) {
      bool ret = false;
      // 上方向に自マスがあるか
      for (int32 i = y - 1; i >= 0; --i) {
        if (board.test(i * _width + x)) {
          ret = true;
          break;
        }
      }

      if (!ret) return false;
      ret = false;

      // 下方向に自マスがあるか
      for (int32 i = y + 1; i < static_cast<int32>(_height); ++i) {
        if (board.test(i * _width + x)) {
          ret = true;
          break;
        }
      }

      if (!ret) return false;
      ret = false;

      // 左方向に自マスがあるか
      for (int32 i = x - 1; i >= 0; --i) {
        if (board.test(y * _width + i)) {
          ret = true;
          break;
        }
      }

      if (!ret) return false;
      ret = false;

      // 右方向に自マスがあるか
      for (int32 i = x + 1; i < static_cast<int32>(_width); ++i) {
        if (board.test(y * _width + i)) {
          ret = true;
          break;
        }
      }

      return ret;
    };

    util::union_find uf(_height * _width);
    for (size_t i = 0; i < _height; ++i) {
      for (size_t j = 0; j < _width; ++j) {
        if (board.test(i * _width + j)) continue;
        for (size_t k = 0; k < 4; ++k) {
          int32 ny = dy[k] + i, nx = dx[k] + j;
          if (!(0 <= ny && ny < static_cast<int32>(_height) && 0 <= nx && nx < static_cast<int32>(_width))) continue;
          if (board.test(ny * _width + nx)) continue;
          uf.unite(i * _width + j, ny * _width + nx);
        }
      }
    }

    boost::unordered_map<int32, std::vector<int32>> area_cand;
    for (size_t i = 0; i < _height; ++i) {
      for (size_t j = 0; j < _width; ++j) {
        if (board.test(i * _width + j)) continue;
        area_cand[uf.root(i * _width + j)].emplace_back(i * _width + j);
      }
    }

    for (auto&& [root, candidates] : area_cand) {
      bool is_area = true;
      int32 local_point = 0;
      for (auto&& cand : candidates) {
        if (!is_between(cand / _width, cand % _width)) {
          is_area = false;
          break;
        }
        local_point += this->field().at(cand / _width, cand % _width);
      }
      if (is_area) ret += local_point;
    }

    return ret;
  }

  int32 all_point(color c) const {
    return tile_point(c) + area_point(c);
  }

  const field_state_hash<_hash_length>& ally_hash() const { return _ally_hash; }
  const field_state_hash<_hash_length>& enemy_hash() const { return _enemy_hash; }
  field_state_hash<_hash_length>& ally_hash() { return _ally_hash; }
  field_state_hash<_hash_length>& enemy_hash() { return _enemy_hash; }
};

}
#endif
