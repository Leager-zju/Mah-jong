#pragma once

#include "Expose.hpp"
#include "Hand.hpp"
#include "Tiles.hpp"

#include <cstdint>
#include <iostream>
#include <memory>

namespace mahjong {
struct MatchResult;

class PlayerTileManager {
 public:
  explicit PlayerTileManager(uint16_t player_index, bool is_my_player)
      : hand_(std::make_unique<Hand>()),
        expose_(std::make_unique<Expose>()),
        player_index_(player_index),
        is_my_player_(is_my_player) {}
  void Draw(pTile new_tile);
  MatchResult TrySelfDrawn(pTile new_tile);

  bool TryChi(pTile new_tile);
  bool TryPong(pTile new_tile, uint16_t discard_player_index);
  bool TryKong(pTile new_tile, uint16_t discard_player_index);
  MatchResult TryWin(pTile new_tile);

  pTile Discard() const;

  void ShowHand() const;
  void ShowExpose() const;
  const Hand& GetHand() const {
    return *hand_;
  }
  const Expose& GetExpose() const {
    return *expose_;
  }

 private:
  std::unique_ptr<Hand> hand_;
  std::unique_ptr<Expose> expose_;

  uint16_t player_index_;
  bool is_my_player_;
  bool in_riichi_ = false;
};
};  // namespace mahjong