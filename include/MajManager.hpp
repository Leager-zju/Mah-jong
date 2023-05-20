#pragma once

#include <cstdint>
#include <memory>

#include "GlobalTileManager.hpp"
#include "PlayerTileManager.hpp"
#include "WinningDetector.hpp"

class MajManager {
 public:
  MajManager();
  void initial();
  void begin();
  void begin_new_round();
  void Win(uint16_t winner_index, bool self_drawn_win, DetectResult detect_result);
  void clear_and_print_header();
  void print_settle_interface();

 private:
  std::unique_ptr<GlobalTileManager> global_tile_manager_;
  std::vector<std::unique_ptr<PlayerTileManager>> player_tile_manager_;

  uint16_t banker_index_;
  uint16_t my_player_index_;
};