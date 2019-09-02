#ifndef INCLUDE_BEAM_SEARCHER_HPP
#define INCLUDE_BEAM_SEARCHER_HPP

#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>

#include <types.hpp>
#include <color.hpp>
#include <util/state_visualize.hpp>

namespace procon30 {
template <typename state_type, size_t beam_width>
class beam_searcher {
private:
  struct node;
  using node_ptr = std::shared_ptr<node>;

  struct node {
    state_type _state;
    node_ptr _parent_node;

    node() {}
    node(const state_type& state, node_ptr parent_node = nullptr) : _state(state), _parent_node(parent_node) {}
    node(state_type&& state, node_ptr parent_node = nullptr) : _state(state), _parent_node(parent_node) {}

    score_type eval() const { return _state.tile_point(); }
    const state_type& state() const { return _state; }
    const node_ptr& parent_node() const { return _parent_node; }
  };

public:
  beam_searcher() {}

  state_type solve(const state_type& init_state) const {
    std::vector<node_ptr> bucket, next;
    bucket.reserve(beam_width * 1000);
    next.reserve(beam_width * 1000);

    auto best = std::make_shared<node>(init_state);

    bucket.emplace_back(best);
    size_t turn = 0;
    while (!bucket.empty()) {
      std::cerr << "Turn: " << turn++ << std::endl;
      next.clear();

      // 一番よい結果
      best = bucket[0];

      for (auto&& e : bucket) {
        auto&& next_states = e->state().next_states();

        for (auto&& st : next_states) {
          next.emplace_back(std::make_shared<node>(std::move(st), e));
        }
      }

      const size_t width = std::min(beam_width, next.size());

      // タイルポイントでソート
      std::partial_sort(next.begin(), next.begin() + width, next.end(),
                        [](const node_ptr& lhs, const node_ptr& rhs) { return lhs->state().tile_point(color::ally) > rhs->state().tile_point(color::ally); });
      next.erase(next.begin() + width, next.end());
      bucket.swap(next);
    }

    std::vector<state_type> res;
    for (auto node = best; node != nullptr; node = node->parent_node()) {
      res.emplace_back(node->state());
    }

    std::reverse(res.begin(), res.end());

    for (auto&& e : res) {
      util::state_visualize(e);
    }

    for (size_t i = 0; i < res.size(); ++i) {
      std::cerr << "Turn: " << i << std::endl;
      util::state_visualize(res.at(i));
    }

    return best->state();
  }
};
}  // namespace procon30

#endif
