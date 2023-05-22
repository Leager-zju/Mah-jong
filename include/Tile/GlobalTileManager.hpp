#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Common.hpp"
#include "Tiles.hpp"

namespace MAHJONG {
class GlobalTileManager {
 public:
  GlobalTileManager();
  static std::unique_ptr<GlobalTileManager>& get_tile_manager();
  void initial();
  pTile pop();
  void receive_discard_tile(pTile discard_tile);
  void show_deck();
  void show_discard_pile();
  void show_dora_indicator();

  bool is_empty() { return head_ > TILE_UPPER_BOUND; }
  bool is_dora(tile_id id);
  bool is_inner_dora(tile_id id);

 private:
  void shuffle();
  static std::unique_ptr<GlobalTileManager> global_tile_manager_;
  uint16_t head_ = TILE_LOWER_BOUND;
  uint16_t tail_ = REPLACEMENT_TILE_UPPER_BOUND - 1;

  uint16_t dora_ = DORA_TILE_UPPER_BOUND - 1;
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
};  // namespace MAHJONG