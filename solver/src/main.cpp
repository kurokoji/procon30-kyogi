#include <agent.hpp>
#include <beam_searcher.hpp>
#include <color.hpp>
#include <field.hpp>
#include <state.hpp>
#include <types.hpp>

#include <util/state_visualize.hpp>

#include <boost/program_options.hpp>

int main(const int argc, const char* const* const argv) {
  using namespace procon30;
  using namespace boost::program_options;

  constexpr size_t hash_length = 10 * 10, agent_count_c = 3;

  if (true) {
    size_t now_turn, max_turn;
    size_t h, w;
    std::cin >> now_turn >> max_turn >> h >> w;
    std::vector<std::vector<int32>> f(h, std::vector<int32>(w));

    for (size_t i = 0; i < h; ++i) {
      for (size_t j = 0; j < w; ++j) {
        std::cin >> f.at(i).at(j);
      }
    }

    size_t agent_count;
    std::cin >> agent_count;

    std::vector<agent> allies, enemies;
    field_state_hash<hash_length> ally_hash, enemy_hash;
    for (size_t i = 0; i < agent_count; ++i) {
      int32 agent_id;
      size_t y, x;
      std::cin >> agent_id >> y >> x;
      ally_hash.set(w * y + x);

      allies.emplace_back(y, x, agent_id, color::ally);
    }

    for (size_t i = 0; i < agent_count; ++i) {
      int32 agent_id;
      size_t y, x;
      std::cin >> agent_id >> y >> x;
      enemy_hash.set(w * y + x);
      enemies.emplace_back(y, x, agent_id, color::enemy);
    }

    using state = normal_state<agent_count_c, hash_length>;
    state init(now_turn, max_turn, field(h, w, f), allies, enemies, ally_hash, enemy_hash);
    beam_searcher<state, agent_count_c> solver;

    auto&& res = solver.solve(init);

    std::cerr << "End" << std::endl;

    util::state_visualize(res);
  } else {
  }
}
