#pragma once

#include <cstddef>
#include <list>
#include <unordered_map>
#include <vector>

#include "Meld.hpp"
#include "Tiles.hpp"
#include "WinningDetector.hpp"

struct DetectResult {
  DetectResult() = default;
  explicit DetectResult(const std::vector<MeldInId>& meld_in_id) : meld_in_id_(meld_in_id) {}
  inline bool has_result() { return !meld_in_id_.empty(); }
  void show_result();
  
  std::vector<MeldInId> meld_in_id_;
};

/**
 * 和牌检测器
 */
class WinningDetector {
 public:
  static DetectResult detect(const class Hand& hand, const class Expose& expose, pTile new_tile);
  static bool dfs(std::unordered_map<tile_id, size_t>& table, std::vector<MeldInId>& melds);
};
