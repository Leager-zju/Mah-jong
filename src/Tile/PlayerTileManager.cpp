#include "PlayerTileManager.hpp"

#include "DoraYaku.hpp"
#include "Tiles.hpp"
#include "YakusMatcher.hpp"

#include <cassert>
#include <iostream>
#include <optional>

namespace mahjong {
void PlayerTileManager::Draw(pTile new_tile) {
  new_tile->SetOwner(player_index_);
  if (is_my_player_) {
    std::cout << "\nYou Draw Tile: " << new_tile->ToString() << '\n';
  }
  hand_->Draw(new_tile);
  // while (is_my_player_) {
  //   int start_index = -1;
  //   hand_->check_concealed_kong(start_index);
  //   if (start_index == -1) break;

  //   if (hand_->try_concealed_kong(start_index)) {
  //     expose_->add_concealed_kong(hand_->do_concealed_kong(start_index));
  //   }
  // }
}

pTile PlayerTileManager::Discard() const {
  pTile discard_tile = nullptr;
  if (is_my_player_) {
    std::string discard_string;
    std::cout << "Input the tile you want to discard(eg. 1m, W): ";
    std::cin >> discard_string;
    while (!hand_->Discard(discard_string, discard_tile)) {
      std::cout << "Wrong Input, please try again(eg. 1m, W): ";
      std::cin >> discard_string;
    }
  } else {
    discard_tile = hand_->RandomDiscard();
  }
  return discard_tile;
}

MatchResult PlayerTileManager::TrySelfDrawn(pTile new_tile) {
  if (MatchResult res = YakusMatcher::TryAllYakuMatch(
          *hand_, *expose_, new_tile, in_riichi_, true);
      res.HasResult()) {
    char option = 0;
    std::cout << "\nYou Can Self-Drawn, Do you? [y/n]";
    std::cin >> option;
    while (option != 'y' && option != 'n') {
      std::cout << "\nWrong Input, Please Try Again! [y/n]";
      std::cin >> option;
    }
    if (option == 'y') {
      Dora::TryMatch(*hand_, *expose_, in_riichi_, res);
      return res;
    }
  }
  return {};
}

void PlayerTileManager::ShowHand() const {
  std::cout << "Your ";
  hand_->Show();
}

void PlayerTileManager::ShowExpose() const {
  std::cout << "Player" << player_index_ << " ";
  expose_->Show();
}
};  // namespace mahjong