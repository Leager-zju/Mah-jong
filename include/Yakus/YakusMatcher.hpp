#pragma once

#include "Common.hpp"
#include "Tiles.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace mahjong {
class Expose;
class Hand;
class MeldInId;

class MatchResult {
 public:
  MatchResult& WithWinningTileId(TileId id) {
    winning_tile_id_ = id;
    return *this;
  }
  inline bool HasResult() const {
    return !yakus_matched_.empty();
  }
  void ShowResult() const;
  void AddYaku(YakuType new_yaku) {
    yakus_matched_.push_back(new_yaku);
  }
  void AddPoint(uint16_t addtion) {
    points_ += addtion;
  }
  void AddYakuman(uint16_t addtion) {
    yakuman_ += addtion;
  }
  void AddDora(uint16_t addtion) {
    dora_ += addtion;
  }
  void AddInnerDora(uint16_t addtion) {
    inner_dora_ += addtion;
  }
  const std::vector<YakuType>& GetYakusMatched() const {
    return yakus_matched_;
  }
  TileId GetWinningTileId() {
    return winning_tile_id_;
  }
  uint16_t GetPoint() const {
    return points_;
  }
  uint16_t GetYakuman() const {
    return yakuman_;
  }
  uint16_t GetDora() const {
    return dora_;
  }
  uint16_t GetInnerDora() const {
    return inner_dora_;
  }

 private:
  std::vector<YakuType> yakus_matched_{};
  TileId winning_tile_id_ = NAT;

  uint16_t points_     = 0;
  uint16_t yakuman_    = 0;
  uint16_t dora_       = 0;
  uint16_t inner_dora_ = 0;
};

class YakusMatcher {
 public:
  static MatchResult TryAllYakuMatch(const Hand& hand,
                                     const Expose& expose,
                                     pTile new_tile,
                                     bool Riichi,
                                     bool self_drawn);
  static void BuildMelds(const Hand& hand,
                         const Expose& expose,
                         pTile new_tile,
                         std::vector<MeldInId>& hand_melds_in_id,
                         std::vector<MeldInId>& expose_melds_in_id);
  static bool FindMeld(std::unordered_map<TileId, size_t>& table,
                       std::vector<MeldInId>& melds);

  static const char* Yaku2String(YakuType yaku);
};
};  // namespace mahjong