#include "MajManager.hpp"

#include "Common.hpp"
#include "Global.hpp"
#include "GlobalTileManager.hpp"
#include "Tiles.hpp"
#include "YakusMatcher.hpp"

#include <Windows.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

namespace mahjong {
const std::unique_ptr<MajManager> MajManager::GLOBAL_MAJ_MANAGER
    = std::make_unique<MajManager>();

MajManager::MajManager() {
  std::random_device rand;
  std::mt19937 mt(rand());
  std::uniform_int_distribution<size_t> dist(0, 3);
  banker_index_    = dist(mt);
  cur_index_       = banker_index_;
  my_player_index_ = dist(mt);

  player_.reserve(TOTAL_PLAYERS);
  for (uint16_t i = 0; i < TOTAL_PLAYERS; i++) {
    player_.emplace_back(
        std::make_unique<PlayerTileManager>(i, i == my_player_index_));
  }
}

const std::unique_ptr<MajManager>& MajManager::GetMajManager() {
  return GLOBAL_MAJ_MANAGER;
}

void MajManager::Initial() {
  auto&& global_tile_manager = GlobalTileManager::GetTileManager();
  global_tile_manager->Initial();
  banker_index_ = NextIndexOf(banker_index_);
  cur_index_    = banker_index_;
  for (size_t round = 0; round < 3; round++) {
    for (size_t i = 0; i < TOTAL_PLAYERS; i++) {
      for (size_t j = 0; j < 4; j++) {
        PlayerDraw(i, global_tile_manager->Pop());
      }
    }
  }

  for (size_t i = 0; i < TOTAL_PLAYERS; i++) {
    PlayerDraw(i, global_tile_manager->Pop());
  }
}

void MajManager::Begin() {
  for (int i = 0; i < 4; i++) {
    BeginNewRound();
  }
}

void MajManager::BeginNewRound() {
  Initial();
  ClearAndPrintHeader();

  uint16_t winner_index = UINT16_MAX;

  bool need_draw        = true;
  bool kong_before_draw = false;
  bool self_drawn_win   = false;

  MatchResult res;

  auto&& global_tile_manager = GlobalTileManager::GetTileManager();
  while (!global_tile_manager->Empty()) {
    pTile new_tile = need_draw
                         ? (kong_before_draw ? global_tile_manager->PopBack()
                                             : global_tile_manager->Pop())
                         : nullptr;

    need_draw        = true;
    kong_before_draw = false;

    res = PlayerDrawAndCheckSelfDrawn(cur_index_, new_tile);
    if (res.HasResult()) {
      self_drawn_win = true;
      winner_index   = my_player_index_;
      break;
    }
    if (player_[cur_index_]->TryKong(new_tile, cur_index_)) {
      kong_before_draw = true;
      // RoundChange(cur_index_);
      continue;
    }

    pTile discard_tile = PlayerDiscard(cur_index_);
    std::cout << "Player" << cur_index_ << " Discard "
              << discard_tile->ToString() << '\n';

    if (cur_index_
        != my_player_index_) {  // TODO(leager): check every player but not myplayer
      res = player_[my_player_index_]->TryWin(discard_tile, false);
      if (res.HasResult()) {
        self_drawn_win = false;
        winner_index   = my_player_index_;
        break;
      }

      if (player_[my_player_index_]->TryKong(discard_tile, cur_index_)) {
        kong_before_draw = true;
        RoundChange(my_player_index_);
        continue;
      }

      if (player_[my_player_index_]->TryPong(discard_tile, cur_index_)) {
        need_draw = false;
        RoundChange(my_player_index_);
        continue;
      }

      if (IsPrevIndexOf(cur_index_, my_player_index_)) {
        if (player_[my_player_index_]->TryChi(discard_tile)) {
          need_draw = false;
          RoundChange(my_player_index_);
          continue;
        }
      }
    }

    PlayerImplementDiscard(cur_index_, discard_tile);

    ClearAndPrintHeader();
    RoundChange(NextIndexOf(cur_index_));
    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  if (winner_index <= 3) {
    std::cout << "Player" << winner_index
              << (self_drawn_win ? " SELF DRAWN!\n" : " RONHO~\n");
    res.ShowResult();
    std::cout << "\n\nPRESS ANY KEY TO CONTINUE...";
    getchar();
    getchar();
  }
}

void MajManager::PlayerDraw(uint16_t index, pTile new_tile) {
  if (new_tile) {
    player_[index]->Draw(new_tile);
  }
}

MatchResult MajManager::PlayerDrawAndCheckSelfDrawn(uint16_t index,
                                                    pTile new_tile) {
  if (new_tile) {
    PlayerDraw(index, new_tile);
    return player_[index]->TryWin(new_tile, true);
  }
  return {};
}

pTile MajManager::PlayerDiscard(uint16_t index) {
  return player_[index]->Discard();
}

void MajManager::PlayerImplementDiscard(uint16_t index, pTile discard_tile) {
  if (discard_tile) {
    player_[index]->ReceiveDiscardTile(discard_tile);
  }
}

void MajManager::ClearAndPrintHeader() const {
  system("cls");
  constexpr auto title = R"(


            ___           ___           ___            ___         ___           ___           ___     
           /\__\         /\  \         /\__\          /\  \       /\  \         /\__\         /\  \    
          /::|  |       /::\  \       /:/  /          \:\  \     /::\  \       /::|  |       /::\  \   
         /:|:|  |      /:/\:\  \     /:/__/       ___ /::\__\   /:/\:\  \     /:|:|  |      /:/\:\  \  
        /:/|:|__|__   /::\~\:\  \   /::\  \ ___  /\  /:/\/__/  /:/  \:\  \   /:/|:|  |__   /:/  \:\  \ 
       /:/ |::::\__\ /:/\:\ \:\__\ /:/\:\  /\__\ \:\/:/  /    /:/__/ \:\__\ /:/ |:| /\__\ /:/__/_\:\__\
       \/__/~~/:/  / \/__\:\/:/  / \/__\:\/:/  /  \::/  /     \:\  \ /:/  / \/__|:|/:/  / \:\  /\ \/__/
             /:/  /       \::/  /       \::/  /    \/__/       \:\  /:/  /      |:/:/  /   \:\ \:\__\  
            /:/  /        /:/  /        /:/  /                  \:\/:/  /       |::/  /     \:\/:/  /  
           /:/  /        /:/  /        /:/  /                    \::/  /        /:/  /       \::/  /   
           \/__/         \/__/         \/__/                      \/__/         \/__/         \/__/    


)";

  std::cout << title;

  auto&& global_tile_manager = GlobalTileManager::GetTileManager();
  global_tile_manager->ShowDoraIndicator();
  for (size_t i = 0; i < 4; i++) {
    std::cout << global::GetColor(i) << "Player " << i << " "
              << "(" << global::GetWind(i, banker_index_) << ")"
              << "(" << global::GetRelativePosition(i, my_player_index_) << ")"
              << global::ColorOff() << '\t';

    player_[i]->ShowDiscard();

    std::cout << " [Expose] ";
    player_[i]->ShowExpose();
    std::cout << '\n';
  }

  std::cout << "Your Hands: ";
  player_[my_player_index_]->ShowHand();
}
};  // namespace mahjong