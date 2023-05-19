#include "WinningDetector.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Meld.hpp"
#include "Tiles.hpp"

void DetectResult::show_result() {
  std::cout << "Result is...\n";
  assert(has_result());
  for (size_t i = 0; i < melds_.size(); i++) {
    melds_[i]->show();
  }
}

DetectResult WinningDetector::detect(const std::list<pTile>& hand) {
  std::vector<std::shared_ptr<Meld>> melds;
  melds.reserve(7);  // 最多组合数为七对子

  std::unordered_map<tile_id_t, size_t> table;
  for (auto&& tile : hand) {
    tile_id_t id = tile->to_id();
    auto iter = table.find(id);
    if (iter == table.end()) {
      table.emplace(id, 1);
    } else {
      iter->second++;
    }
  }
  for (auto iter = table.begin(); iter != table.end(); iter++) {
    if (iter->second >= 2) {
      iter->second -= 2;
      melds.emplace_back(std::make_shared<Eyes>(iter->first));
      if (dfs(table, melds)) {
        break;
      }
      melds.pop_back();
      iter->second += 2;
    }
  }
  return DetectResult(melds);
}

bool WinningDetector::dfs(std::unordered_map<tile_id_t, size_t>& table, std::vector<std::shared_ptr<Meld>>& melds) {
  for (auto iter = table.begin(); iter != table.end(); iter++) {
    if (iter->second >= 3) {
      iter->second -= 3;
      melds.emplace_back(std::make_shared<Triplet>(iter->first));
      if (dfs(table, melds)) {
        return true;
      }
      melds.pop_back();
      iter->second += 3;
    }

    if (iter->second >= 1) {
      auto iter_plus1 = table.find(iter->first + 1);
      auto iter_plus2 = table.find(iter->first + 2);
      if (iter_plus1 == table.end() || iter_plus1->second == 0 || iter_plus2 == table.end() ||
          iter_plus2->second == 0) {
        continue;
      }
      iter->second--;
      iter_plus1->second--;
      iter_plus2->second--;
      melds.emplace_back(std::make_shared<Sequence>(iter->first, iter_plus1->first, iter_plus2->first));
      if (dfs(table, melds)) {
        return true;
      }
      melds.pop_back();
      iter->second++;
      iter_plus1->second++;
      iter_plus2->second++;
    }
  }

  return false;
}