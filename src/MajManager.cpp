#include "MajManager.hpp"

#include <corecrt.h>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <thread>

#include "PlayerTileManager.hpp"
#include "Tiles.hpp"
#include "WinningDetector.hpp"

MajManager::MajManager()
    : global_tile_manager_(std::make_unique<GlobalTileManager>()),
      banker_index_(std::rand() % 4),
      my_player_index_(std::rand() % 4) {
  player_tile_manager_.reserve(4);
  for (uint16_t i = 0; i < 4; i++) {
    player_tile_manager_.emplace_back(std::make_unique<PlayerTileManager>(i, i == my_player_index_));
  }
}

void MajManager::initial() {
  global_tile_manager_->initial();
  banker_index_ = (banker_index_ + 1) % 4;
  for (size_t round = 0; round < 3; round++) {
    for (size_t i = 0; i < 4; i++) {
      for (size_t j = 0; j < 4; j++) {
        player_tile_manager_[i]->draw(global_tile_manager_->pop());
      }
    }
  }

  for (size_t i = 0; i < 4; i++) {
    player_tile_manager_[i]->draw(global_tile_manager_->pop());
  }

  // TODO(Leager): 翻开一张宝牌指示牌
}

void MajManager::begin() {
  for (int i = 0; i < 4; i++) {
    begin_new_round();
  }
}

void MajManager::begin_new_round() {
  initial();
  clear_and_print_header();
  size_t index = banker_index_;
  
  bool self_drawn_win = false;
  size_t winner_index = UINT16_MAX;
  DetectResult res;
  
  while (!global_tile_manager_->is_empty()) {
    pTile new_tile = global_tile_manager_->pop();
    player_tile_manager_[index]->draw(new_tile);

    res = player_tile_manager_[index]->try_self_drawn(new_tile);
    if (res.has_result()) {
      self_drawn_win = true;
      winner_index = index;
      break;
    }

    pTile discard_tile = player_tile_manager_[index]->discard();
    std::string&& discard_tile_string = discard_tile->to_string();
    global_tile_manager_->receive_discard_tile(discard_tile);

    clear_and_print_header();
    std::cout << "Player" << index << " Discard " << discard_tile_string << '\n';
    std::this_thread::sleep_for(std::chrono::seconds(3));
    index = (index + 1) % 4;
  }

  if (winner_index <= 3) {
    Win(winner_index, self_drawn_win, res);
  }
}

void MajManager::Win(uint16_t winner_index, bool self_drawn_win, DetectResult detect_result) {
  std::cout << "Player" << winner_index << (self_drawn_win ? " SELF_DRAWN!\n" : " RONHO~\n");
  print_settle_interface();
}

void MajManager::clear_and_print_header() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
  std::cout << "You're Player" << my_player_index_ << '\n';
  global_tile_manager_->show_discard_pile();
  global_tile_manager_->show_dora_indicator();
  for (size_t i = 0; i < 4; i++) {
    if (i == my_player_index_) continue;

    player_tile_manager_[i]->show_expose();
  }
  player_tile_manager_[my_player_index_]->show_hand();
}

void MajManager::print_settle_interface() {
  
}