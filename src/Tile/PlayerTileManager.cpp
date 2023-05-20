#include "PlayerTileManager.hpp"

#include <cassert>
#include <iostream>
#include <optional>

#include "Tiles.hpp"
#include "WinningDetector.hpp"

void PlayerTileManager::draw(pTile new_tile) {
  new_tile->set_owner(player_index_);
  if (is_my_player_) {
    std::cout << "\nYou Draw Tile: " << new_tile->to_string() << '\n';
  }
  hand_->draw(new_tile);
  // while (is_my_player_) {
  //   int start_index = -1;
  //   hand_->check_concealed_kong(start_index);
  //   if (start_index == -1) break;

  //   if (hand_->try_concealed_kong(start_index)) {
  //     expose_->add_concealed_kong(hand_->do_concealed_kong(start_index));
  //   }
  // }
}

pTile PlayerTileManager::discard() {
  pTile discard_tile;
  if (is_my_player_) {
    std::string discard_string;
    std::cout << "Input the tile you want to discard(eg. 1m, W): ";
    std::cin >> discard_string;
    while (!hand_->discard(discard_string, discard_tile)) {
      std::cout << "Wrong Input, please try again(eg. 1m, W): ";
      std::cin >> discard_string;
    }
  } else {
    discard_tile = hand_->random_discard();
  }
  return discard_tile;
}

DetectResult PlayerTileManager::try_self_drawn(pTile new_tile) {
  DetectResult res = WinningDetector::detect(*hand_, *expose_, new_tile);
  if (res.has_result()) {
    char option;
    std::cout << "\nYou Can Self-Drawn, Do you? [y/n]";
    std::cin >> option;
    assert(option == 'y' || option == 'n');
    return option == 'y' ? res : DetectResult{};
  }
  return res;
}


void PlayerTileManager::show_hand() {
  std::cout << "Your ";
  hand_->show();
}

void PlayerTileManager::show_expose() {
  std::cout << "Player" << player_index_ << " ";
  expose_->show();
}