#include "WinningDetector.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Expose.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"

void DetectResult::show_result() {
  std::cout << "Result is...\n";
  assert(has_result());
  for (auto&& meld : meld_in_id_) {
    meld.show();
  }
}

DetectResult WinningDetector::detect(const Hand& hand, const Expose& expose, pTile new_tile) {
  std::vector<MeldInId> melds_in_id;
  melds_in_id.reserve(7);  // 最多组合数为七对子

  std::cout << "Detect...\n";
  hand.show();
  expose.show();
  std::cout << "New Tile: " << new_tile->to_string() << '\n';
  // make table
  std::unordered_map<tile_id, size_t> table;
  for (auto&& tile : hand.get_hands()) {
    tile_id id = tile->to_id();
    auto&& iter = table.find(id);
    if (iter == table.end()) {
      table.emplace(id, 1);
    } else {
      iter->second++;
    }
  }
  tile_id new_tile_id = new_tile->to_id();
  auto&& new_iter = table.find(new_tile_id);
  if (new_iter == table.end()) {
    table.emplace(new_tile_id, 1);
  } else {
    new_iter->second++;
  }
  for (auto&& meld : expose.get_exposes()) {
    melds_in_id.push_back(MeldInId(meld));
  }

  for (auto&& iter = table.begin(); iter != table.end(); iter++) {
    if (iter->second >= 2) {
      iter->second -= 2;
      melds_in_id.push_back(MeldInId(MeldType::Eyes, iter->first, iter->first));
      if (dfs(table, melds_in_id)) {
        break;
      }
      melds_in_id.pop_back();
      iter->second += 2;
    }
  }
  return DetectResult(melds_in_id);
}

bool WinningDetector::dfs(std::unordered_map<tile_id, size_t>& table, std::vector<MeldInId>& melds) {
  bool clear = true;
  for (auto&& iter = table.begin(); iter != table.end(); iter++) {
    if (iter->second) {
      clear = false;
    }
    if (iter->second >= 3) {
      iter->second -= 3;
      melds.push_back(MeldInId(MeldType::Triplet, iter->first, iter->first, iter->first));
      if (dfs(table, melds)) {
        return true;
      }
      melds.pop_back();
      iter->second += 3;
    }

    if (iter->second >= 1) {
      auto&& iter_plus1 = table.find(static_cast<tile_id>(iter->first + 1));
      auto&& iter_plus2 = table.find(static_cast<tile_id>(iter->first + 2));
      if (iter_plus1 == table.end() || iter_plus1->second == 0 || iter_plus2 == table.end() ||
          iter_plus2->second == 0) {
        continue;
      }
      iter->second--;
      iter_plus1->second--;
      iter_plus2->second--;
      melds.push_back(MeldInId(MeldType::Sequence, iter->first, iter_plus1->first, iter_plus2->first));
      if (dfs(table, melds)) {
        return true;
      }
      melds.pop_back();
      iter->second++;
      iter_plus1->second++;
      iter_plus2->second++;
    }
  }

  return clear;
}