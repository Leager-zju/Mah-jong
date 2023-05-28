#pragma once

#include "Common.hpp"

#include <set>
#include <unordered_map>

namespace mahjong {
class Hand;
class Expose;
class MeldInId;
using pTile = class Tile*;

class WaitingResult {
 public:
  explicit WaitingResult(TileId discard, std::set<TileId> waiting_for)
      : discard_(discard), waiting_for_(std::move(waiting_for)) {}
  void Append(std::set<TileId>&& waiting_for);
  TileId GetDiscard() const {
    return discard_;
  }
  const std::set<TileId>& GetWaitingFor() const {
    return waiting_for_;
  }

 private:
  TileId discard_;
  std::set<TileId> waiting_for_;
};

namespace riichi_detector {
  std::vector<WaitingResult> CheckRiichi(const Hand& hand,
                                         const Expose& expose);
  bool CheckThirteenOrphans(std::unordered_map<TileId, size_t>& table,
                            std::vector<WaitingResult>& results);
  bool CheckSevenPairs(std::unordered_map<TileId, size_t>& table,
                       std::vector<WaitingResult>& results);
  void Reduce(std::unordered_map<TileId, size_t>& table,
              bool find_eyes,
              std::vector<WaitingResult>& results,
              std::unordered_map<TileId, size_t>& first_index);

}  // namespace riichi_detector
}  // namespace mahjong
