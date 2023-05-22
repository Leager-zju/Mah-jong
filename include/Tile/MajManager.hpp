#pragma once

#include "PlayerTileManager.hpp"

#include <cstdint>
#include <memory>

namespace mahjong {
struct MatchResult;

class MajManager {
 public:
  MajManager();
  static const std::unique_ptr<MajManager>& GetMajManager();
  void Initial();
  void Begin();
  void BeginNewRound();
  void ClearAndPrintHeader() const;

  uint16_t GetBankerIndex() const {
    return banker_index_;
  }
  uint16_t GetMyPlayerIndex() const {
    return my_player_index_;
  }
  uint16_t GetSeatWind() const {
    return seat_wind_;
  }

 private:
  static const std::unique_ptr<MajManager> GLOBAL_MAJ_MANAGER;
  std::vector<std::unique_ptr<PlayerTileManager>> player_tile_manager_;

  uint16_t banker_index_;
  uint16_t my_player_index_;
  uint16_t seat_wind_ = 0;  // 0/1/2/3 for 东/南/西/北
};
};  // namespace mahjong