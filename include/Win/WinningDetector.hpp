#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <vector>

#include "Meld.hpp"
#include "Tiles.hpp"
#include "WinningDetector.hpp"

struct DetectResult {
  explicit DetectResult(const std::vector<std::shared_ptr<Meld>>& melds) : melds_(melds) {}
  std::vector<std::shared_ptr<Meld>> melds_;
  inline bool has_result() { return !melds_.empty(); }
  void show_result();
};

/**
 * 和牌检测器
 */
class WinningDetector {
  static DetectResult detect(const std::list<pTile>& hand);
  static bool dfs(std::unordered_map<tile_id_t, size_t>& table, std::vector<std::shared_ptr<Meld>>& melds);
};
