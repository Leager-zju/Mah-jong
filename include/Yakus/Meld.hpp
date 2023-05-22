#pragma once

#include "Common.hpp"
#include "Tiles.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace mahjong {
enum class MeldType : uint16_t {
  Eyes,           // 雀头
  Triplet,        // 刻子
  Sequence,       // 顺子
  ExposeKong,     // 明杠
  ConcealedKong,  // 暗杠
};

class Meld {
 public:
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
  explicit Meld(MeldType meld_type,
                pTile tile_1,
                pTile tile_2,
                pTile tile_3,
                pTile tile_4)
      : Meld(meld_type, tile_1, tile_2, tile_3) {
    meld_.push_back(tile_4);
  }
  void Show() const;

  MeldType GetMeldType() const {
    return meld_type_;
  }
  const std::vector<pTile>& GetMeld() const {
    return meld_;
  }

 private:
  MeldType meld_type_;
  std::vector<pTile> meld_;
};

// used in winning detector
class MeldInId {
 public:
  explicit MeldInId(const Meld& other) : meld_type_(other.GetMeldType()) {
    for (auto tile : other.GetMeld()) {
      tile_ids_.push_back(tile->ToId());
    }
  }
  // Eyes
  explicit MeldInId(MeldType meld_type, TileId tile_id1, TileId tile_id2)
      : meld_type_(meld_type) {
    tile_ids_.push_back(tile_id1);
    tile_ids_.push_back(tile_id2);
  }
  // Triplet/Sequence
  explicit MeldInId(MeldType meld_type,
                    TileId tile_id1,
                    TileId tile_id2,
                    TileId tile_id3)
      : MeldInId(meld_type, tile_id1, tile_id2) {
    tile_ids_.push_back(tile_id3);
  }
  // ExposeKong/ConcealedKong
  explicit MeldInId(MeldType meld_type,
                    TileId tile_id1,
                    TileId tile_id2,
                    TileId tile_id3,
                    TileId tile_id4)
      : MeldInId(meld_type, tile_id1, tile_id2, tile_id3) {
    tile_ids_.push_back(tile_id4);
  }
  void Show() const;
  const MeldType& GetMeldType() const {
    return meld_type_;
  }
  const std::vector<TileId>& GetTileId() const {
    return tile_ids_;
  }

 private:
  MeldType meld_type_;
  std::vector<TileId> tile_ids_;
};
};  // namespace mahjong