#pragma once

#include "Common.hpp"
#include "Tiles.hpp"

#include <cassert>
#include <cstdint>
#include <list>
#include <optional>
#include <unordered_map>
#include <utility>
#include <vector>

namespace mahjong {
using pTile     = class Tile*;
using Tile_Iter = std::list<pTile>::iterator;
/**
 * 手牌
 */
class Hand {
 public:
  Hand() = default;
  explicit Hand(std::list<pTile> in_hand) : in_hand_(std::move(in_hand)) {}
  void Initial() {
    in_hand_.clear();
    last_draw_ = in_hand_.end();
  }
  void Draw(pTile new_tile);
  bool Discard(const std::string& discard_string, pTile& discard_tile);
  pTile RandomDiscard();
  pTile DiscardLastDraw();
  void Show() const;

  const std::list<pTile>& GetHands() const {
    return in_hand_;
  }

  pTile GetLastDraw() const {
    return *last_draw_;
  }

  void RemoveTile(TileId id, uint16_t count);
  void RemoveTiles(const std::vector<Tile_Iter>& iters);
  Tile_Iter FindTile(TileId id);
  std::vector<Tile_Iter> FindTile(TileId id, uint16_t count);

 private:
  std::list<pTile> in_hand_;
  Tile_Iter last_draw_ = in_hand_.end();
};
};  // namespace mahjong