#include "MajManager.hpp"

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
  my_player_index_ = dist(mt);
  player_tile_manager_.reserve(4);
  for (uint16_t i = 0; i < 4; i++) {
    player_tile_manager_.emplace_back(
        std::make_unique<PlayerTileManager>(i, i == my_player_index_));
  }
}

const std::unique_ptr<MajManager>& MajManager::GetMajManager() {
  return GLOBAL_MAJ_MANAGER;
}

void MajManager::Initial() {
  auto&& global_tile_manager = GlobalTileManager::GetTileManager();
  global_tile_manager->Initial();
  banker_index_ = (banker_index_ + 1) % 4;
  for (size_t round = 0; round < 3; round++) {
    for (size_t i = 0; i < 4; i++) {
      for (size_t j = 0; j < 4; j++) {
        player_tile_manager_[i]->Draw(global_tile_manager->Pop());
      }
    }
  }

  for (size_t i = 0; i < 4; i++) {
    player_tile_manager_[i]->Draw(global_tile_manager->Pop());
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
  size_t index = banker_index_;

  bool self_drawn_win = false;
  size_t winner_index = UINT16_MAX;
  MatchResult res;

  auto&& global_tile_manager = GlobalTileManager::GetTileManager();
  while (!global_tile_manager->IsEmpty()) {
    pTile new_tile = global_tile_manager->Pop();

    player_tile_manager_[index]->Draw(new_tile);
    if (index == my_player_index_) {
      res = player_tile_manager_[index]->TrySelfDrawn(new_tile);
      if (res.HasResult()) {
        self_drawn_win = true;
        winner_index   = my_player_index_;
        break;
      }
      player_tile_manager_[index]->TryKong(new_tile, index);
    }

    pTile discard_tile = player_tile_manager_[index]->Discard();
    std::cout << "Player" << index << " Discard " << discard_tile->ToString()
              << '\n';
    bool do_operation = false;
    if (index != my_player_index_) {
      res = player_tile_manager_[my_player_index_]->TryWin(discard_tile);
      if (res.HasResult()) {
        self_drawn_win = false;
        winner_index   = my_player_index_;
        break;
      }

      do_operation = player_tile_manager_[my_player_index_]->TryKong(
          discard_tile, index);
      if (!do_operation) {
        do_operation = player_tile_manager_[my_player_index_]->TryPong(
            discard_tile, index);
        if (!do_operation && index == (my_player_index_ + 3) % 4) {
          do_operation
              = player_tile_manager_[my_player_index_]->TryChi(discard_tile);
        }
      }
    }

    if (!do_operation) {
      global_tile_manager->ReceiveDiscardTile(discard_tile);
      ClearAndPrintHeader();
      index = (index + 1) % 4;
    } else {
      ClearAndPrintHeader();
      index = (my_player_index_ + 1) % 4;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  if (winner_index <= 3) {
    std::cout << "Player" << winner_index
              << (self_drawn_win ? " SELF DRAWN!\n" : " RONHO~\n");
    res.ShowResult();
    std::cout << "\n\nPRESS ANY KEY TO CONTINUE...";
    getchar();
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
  std::cout << "You're" << global::GetColor(my_player_index_) << " Player "
            << my_player_index_ << "("
            << global::GetWind(my_player_index_, banker_index_) << ")"
            << global::ColorOff() << '\n';
  global_tile_manager->ShowDiscardPile();
  global_tile_manager->ShowDoraIndicator();
  for (size_t i = 0; i < 4; i++) {
    if (i == my_player_index_) {
      continue;
    }

    std::cout << global::GetColor(i) << "Player " << i << " "
              << "(" << global::GetWind(i, banker_index_) << ")"
              << "(" << global::GetRelativePosition(i, my_player_index_) << ")"
              << global::ColorOff();

    player_tile_manager_[i]->ShowExpose();
  }
  player_tile_manager_[my_player_index_]->ShowHand();
}
};  // namespace mahjong