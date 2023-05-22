#include "MajManager.hpp"

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
  std::uniform_real_distribution<size_t> dist(4);
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

  // TODO(Leager): 翻开一张宝牌指示牌
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

    res = player_tile_manager_[index]->TrySelfDrawn(new_tile);
    if (res.HasResult()) {
      self_drawn_win = true;
      winner_index   = index;
      break;
    }

    pTile discard_tile                = player_tile_manager_[index]->Discard();
    std::string&& discard_tile_string = discard_tile->ToString();
    global_tile_manager->ReceiveDiscardTile(discard_tile);

    ClearAndPrintHeader();
    std::cout << "Player" << index << " Discard " << discard_tile_string
              << '\n';
    std::this_thread::sleep_for(std::chrono::seconds(3));
    index = (index + 1) % 4;
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
  const int clear_lines      = 50;
  auto&& global_tile_manager = GlobalTileManager::GetTileManager();
  std::cout << std::string(clear_lines, '\n');
  std::cout << "You're Player" << my_player_index_ << '\n';
  global_tile_manager->ShowDiscardPile();
  global_tile_manager->ShowDoraIndicator();
  for (size_t i = 0; i < 4; i++) {
    if (i == my_player_index_) {
      continue;
    }

    player_tile_manager_[i]->ShowExpose();
  }
  player_tile_manager_[my_player_index_]->ShowHand();
}
};  // namespace mahjong