#pragma once

#include "Common.hpp"
#include "PlayerTileManager.hpp"

#include <cstdint>
#include <memory>

namespace mahjong {
struct WinningResult;
using pTile = class Tile*;

class MajManager {
 public:
  MajManager();
  static const std::unique_ptr<MajManager>& GetMajManager();
  static uint16_t NextIndexOf(uint16_t index) {
    return (index + 1) % TOTAL_PLAYERS;
  }
  static bool IsPrevIndexOf(uint16_t other, uint16_t index) {
    return other == (index + 3) % TOTAL_PLAYERS;
  }

  void Initial();
  void Begin();
  void BeginNewRound();
  void PlayerDraw(uint16_t index, pTile new_tile);
  WinningResult PlayerDrawAndCheckSelfDrawn(uint16_t index, pTile new_tile);
  void PlayerTryRiichi(uint16_t index, pTile& discard_tile);
  void PlayerDiscard(uint16_t index, pTile& discard_tile);
  void PlayerImplementDiscard(uint16_t index, pTile discard_tile);

  static void ClearAndPrintTitle();
  static bool IsValidPlayerIndex(uint16_t index) {
    return index >= 0 && index <= 3;
  }
  void ClearAndPrintHeader() const;

  void RoundChange(uint16_t next_player_index) {
    cur_index_ = next_player_index;
  }
  uint16_t GetBankerIndex() const {
    return banker_index_;
  }
  uint16_t GetMyPlayerIndex() const {
    return my_player_index_;
  }
  uint16_t GetSeatWind() const {
    return seat_wind_;
  }

  // !!! TEST ONLY !!!
  void SetBankerIndex(uint16_t index) {
    banker_index_ = index;
  }
  // !!! TEST ONLY !!!
  void SetMyPlayerIndex(uint16_t index) {
    my_player_index_ = index;
  }

 private:
  static const std::unique_ptr<MajManager> GLOBAL_MAJ_MANAGER;
  std::vector<std::unique_ptr<PlayerTileManager>> players_;

  uint16_t cur_index_;
  uint16_t banker_index_;
  uint16_t my_player_index_;
  uint16_t seat_wind_ = 0;  // 0/1/2/3 for 东/南/西/北
};
};  // namespace mahjong