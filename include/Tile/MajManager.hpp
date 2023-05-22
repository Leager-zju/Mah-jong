#pragma once

#include <cstdint>
#include <memory>

#include "GlobalTileManager.hpp"
#include "PlayerTileManager.hpp"

namespace MAHJONG {
struct MatchResult;

class MajManager {
 public:
  MajManager();
  static std::unique_ptr<MajManager>& get_maj_manager();
  void initial();
  void begin();
  void begin_new_round();
  void Win(uint16_t winner_index, bool self_drawn_win,
           const MatchResult& detect_result);
  void clear_and_print_header();

  uint16_t get_banker_index() { return banker_index_; }
  uint16_t get_my_player_index() { return my_player_index_; }
  uint16_t get_seat_wind() { return seat_wind_; }

 private:
  static std::unique_ptr<MajManager> global_maj_manager_;
  std::vector<std::unique_ptr<PlayerTileManager>> player_tile_manager_;

  uint16_t banker_index_;
  uint16_t my_player_index_;
  uint16_t seat_wind_ = 0;  // 0/1/2/3 for 东/南/西/北
};
};  // namespace MAHJONG