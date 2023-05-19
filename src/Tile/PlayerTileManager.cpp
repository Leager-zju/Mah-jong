#include "PlayerTileManager.hpp"

#include <iostream>
#include <optional>

#include "Tiles.hpp"

void PlayerTileManager::draw(pTile new_tile) {
  new_tile->set_owner(player_index_);
  if (is_my_player_) {
    std::cout << "\nYou Draw Tile: " << new_tile->to_string() << '\n';
  }
  hand_->draw(std::move(new_tile));
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

void PlayerTileManager::show_hand() {
  std::cout << "Your ";
  hand_->show();
}

void PlayerTileManager::show_expose() {
  std::cout << "Player" << player_index_ << " ";
  expose_->show();
}