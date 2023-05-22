#pragma once

#include <cstdint>
#include <string>

#include "Tiles.hpp"

namespace MAHJONG {
class Expose;
class Hand;
class MeldInId;

struct MatchResult {
  MatchResult& with_winning_tile_id(tile_id id) {
    winning_tile_id_ = id;
    return *this;
  }
  inline bool has_result() { return !Yakus_matched_.empty(); }
  void show_result() const;

  std::vector<YakuType> Yakus_matched_{};
  tile_id winning_tile_id_ = NAT;

  uint16_t points_ = 0;
  uint16_t yakuman_ = 0;
  uint16_t dora_ = 0;
  uint16_t inner_dora_ = 0;
};

class YakuMatcher {
 public:
  static MatchResult detect(const Hand& hand, const Expose& expose,
                            pTile new_tile, bool Riichi, bool self_drawn);
  static MatchResult try_all_yaku_match(const Hand& hand, const Expose& expose,
                                        pTile new_tile, bool Riichi,
                                        bool self_drawn);
  static void build_melds(const Hand& hand, const Expose& expose,
                          pTile new_tile,
                          std::vector<MeldInId>& hand_melds_in_id,
                          std::vector<MeldInId>& expose_melds_in_id);
  static bool find_meld(std::unordered_map<tile_id, size_t>& table,
                        std::vector<MeldInId>& melds);

  static const char* Yaku2String(YakuType yaku);
};
};  // namespace MAHJONG