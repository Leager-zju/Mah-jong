#pragma once

#include <cstdint>
#include <iostream>
#include <memory>

#include "Expose.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"
#include "WinningDetector.hpp"

class PlayerTileManager {
 public:
  explicit PlayerTileManager(uint16_t player_index, bool is_my_player)
      : hand_(std::make_unique<Hand>()),
        expose_(std::make_unique<Expose>()),
        player_index_(player_index),
        is_my_player_(is_my_player) {}
  void draw(pTile new_tile);
  DetectResult try_self_drawn(pTile new_tile);

  pTile discard();
  void show_hand();
  void show_expose();

 private:
  std::unique_ptr<Hand> hand_;
  std::unique_ptr<Expose> expose_;

  uint16_t player_index_;
  bool is_my_player_;
};