#pragma once

#include "Common.hpp"
#include "Tiles.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace mahjong {
class GlobalTileManager {
 public:
  GlobalTileManager();
  static const std::unique_ptr<GlobalTileManager>& GetTileManager();
  void Initial();
  pTile Pop();
  void ReceiveDiscardTile(pTile discard_tile);
  void ShowDeck() const;
  void ShowDiscardPile() const;
  void ShowDoraIndicator() const;

  bool IsEmpty() const {
    return head_ > TILE_UPPER_BOUND;
  }
  bool IsDora(TileId id) const;
  bool IsInnerDora(TileId id) const;

 private:
  void Shuffle();
  static const std::unique_ptr<GlobalTileManager> GLOBAL_TILE_MANAGER;
  uint16_t head_ = TILE_LOWER_BOUND;
  uint16_t tail_ = REPLACEMENT_TILE_UPPER_BOUND - 1;

  uint16_t dora_       = DORA_TILE_UPPER_BOUND - 1;
  uint16_t inner_dora_ = DORA_TILE_UPPER_BOUND;

  /*
   * 可抽牌(0-121)
   *       | 宝牌指示牌   122 124 126 128 130 |       132 134 |
   * 王牌  | 里宝牌指示牌 123 125 127 129 131 | 岭上牌 133 135 |
   */
  std::vector<pTile> deck_;

  /*
   * 牌河
   */
  std::vector<pTile> disacrd_pile_;
};
};  // namespace mahjong