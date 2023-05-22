#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "Common.hpp"
#include "Tiles.hpp"

namespace MAHJONG {
enum class MeldType : uint16_t {
  Eyes,           // 雀头
  Triplet,        // 刻子
  Sequence,       // 顺子
  ExposeKong,     // 明杠
  ConcealedKong,  // 暗杠
};

struct Meld {
  // Eyes
  explicit Meld(MeldType meld_type, pTile tile_1, pTile tile_2)
      : meld_type_(meld_type) {
    meld_.push_back(tile_1);
    meld_.push_back(tile_2);
  }
  // Triplet/Sequence
  explicit Meld(MeldType meld_type, pTile tile_1, pTile tile_2, pTile tile_3)
      : Meld(meld_type, tile_1, tile_2) {
    meld_.push_back(tile_3);
  }
  // ExposeKong/ConcealedKong
  explicit Meld(MeldType meld_type, pTile tile_1, pTile tile_2, pTile tile_3,
                pTile tile_4)
      : Meld(meld_type, tile_1, tile_2, tile_3) {
    meld_.push_back(tile_4);
  }

  void show() const;
  MeldType meld_type_;
  std::vector<pTile> meld_;
};

// used in winning detector
struct MeldInId {
  explicit MeldInId(const Meld& meld) {
    for (pTile p : meld.meld_) {
      tile_ids_.push_back(p->to_id());
    }
  }
  // Eyes
  explicit MeldInId(MeldType meld_type, tile_id tile_id1, tile_id tile_id2)
      : meld_type_(meld_type) {
    tile_ids_.push_back(tile_id1);
    tile_ids_.push_back(tile_id2);
  }
  // Triplet/Sequence
  explicit MeldInId(MeldType meld_type, tile_id tile_id1, tile_id tile_id2,
                    tile_id tile_id3)
      : MeldInId(meld_type, tile_id1, tile_id2) {
    tile_ids_.push_back(tile_id3);
  }
  // ExposeKong/ConcealedKong
  explicit MeldInId(MeldType meld_type, tile_id tile_id1, tile_id tile_id2,
                    tile_id tile_id3, tile_id tile_id4)
      : MeldInId(meld_type, tile_id1, tile_id2, tile_id3) {
    tile_ids_.push_back(tile_id4);
  }

  void show() const;
  MeldType meld_type_;
  std::vector<tile_id> tile_ids_;
};
};  // namespace MAHJONG