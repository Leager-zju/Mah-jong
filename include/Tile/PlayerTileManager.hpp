#pragma once

#include "Expose.hpp"
#include "Hand.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

namespace mahjong {
struct WinningResult;
using pTile = class Tile*;

class PlayerTileManager {
 public:
  explicit PlayerTileManager(uint16_t player_index, bool is_my_player)
      : hand_(std::make_unique<Hand>()),
        expose_(std::make_unique<Expose>()),
        player_index_(player_index),
        is_my_player_(is_my_player) {}
  void Initial();
  void Draw(pTile new_tile);
  void ReceiveDiscardTile(pTile discard_tile);

  WinningResult TryWin(pTile new_tile, bool self_drawn);
  pTile TryRiichi();
  bool InRiichi() const {
    return in_riichi_;
  }

  bool TryChi(pTile new_tile);
  bool TryPong(pTile new_tile, uint16_t discard_player_index);
  bool TryKong(pTile new_tile, uint16_t discard_player_index);

  pTile Discard() const;

  void ShowHand() const;
  void ShowExpose() const;
  void ShowDiscard() const;

  const Hand& GetHand() const {
    return *hand_;
  }
  const Expose& GetExpose() const {
    return *expose_;
  }

 private:
  std::unique_ptr<Hand> hand_;
  std::unique_ptr<Expose> expose_;
  std::vector<pTile> discards_;

  uint16_t player_index_;
  bool is_my_player_;
  bool in_riichi_ = false;
};
};  // namespace mahjong