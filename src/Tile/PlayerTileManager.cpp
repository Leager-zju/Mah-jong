#include "PlayerTileManager.hpp"


#include "Common.hpp"
#include "DoraYaku.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"
#include "YakusMatcher.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <unordered_set>
#include <vector>


namespace mahjong {
void PlayerTileManager::Draw(pTile new_tile) {
  new_tile->SetOwner(player_index_);
  if (is_my_player_) {
    std::cout << "\nYou Draw Tile: " << new_tile->ToString() << '\n';
  }
  hand_->Draw(new_tile);
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

bool PlayerTileManager::TryChi(pTile new_tile) {
  if (Tile::IsHonor(new_tile->GetId())) {
    return false;
  }

  std::vector<Meld> possible_melds;
  std::unordered_map<TileId, std::list<pTile>::iterator> table{};

  auto hands = hand_->GetHands();
  auto iter1 = hands.end();
  auto iter2 = hands.end();

  for (auto iter = hands.begin(); iter != hands.end(); iter++) {
    TileId id = (*iter)->GetId();
    if (!table.count(id)) {
      table.insert({id, iter});
    }
  }
  TileId new_tile_id = new_tile->GetId();
  uint16_t rank      = new_tile_id % 10;
  if (rank <= 7) {
    auto plus1 = static_cast<TileId>(new_tile_id + 1);
    auto plus2 = static_cast<TileId>(new_tile_id + 2);
    if (table.count(plus1) && table.count(plus2)) {
      iter1 = table[plus1];
      iter2 = table[plus2];
      possible_melds.emplace_back(MeldType::Sequence, new_tile, *iter1, *iter2);
    }
  }
  if (rank >= 2 && rank <= 8) {
    auto minus1 = static_cast<TileId>(new_tile_id - 1);
    auto plus1  = static_cast<TileId>(new_tile_id + 1);
    if (table.count(minus1) && table.count(plus1)) {
      iter1 = table[minus1];
      iter2 = table[plus1];
      possible_melds.emplace_back(MeldType::Sequence, *iter1, new_tile, *iter2);
    }
  }
  if (rank >= 3) {
    auto minus2 = static_cast<TileId>(new_tile_id - 2);
    auto minus1 = static_cast<TileId>(new_tile_id - 1);
    if (table.count(minus1) && table.count(minus2)) {
      iter1 = table[minus2];
      iter2 = table[minus1];
      possible_melds.emplace_back(MeldType::Sequence, *iter1, *iter2, new_tile);
    }
  }
  if (possible_melds.empty()) {
    return false;
  }

  size_t select_index = -1;
  std::cout << "You Can Do \"Chi\" Operation:\n";
  std::cout << "1. " << possible_melds[0].ToString() << '\n';
  if (possible_melds.size() >= 2) {
    std::cout << "2. " << possible_melds[1].ToString() << '\n';
  }
  if (possible_melds.size() >= 3) {
    std::cout << "3. " << possible_melds[2].ToString() << '\n';
  }
  std::cout << "Choose A Meld You Want To Chi, 0 for cancel. ";
  std::cin >> select_index;
  while (select_index > possible_melds.size()) {
    std::cout << "Wrong Input, Please Try Again. ";
    std::cin >> select_index;
  }

  if (select_index == 0) {
    return false;
  }

  expose_->AddNewMeld(possible_melds[select_index - 1]);
  hand_->RemoveTile(iter1);
  hand_->RemoveTile(iter2);
  return true;
}

bool PlayerTileManager::TryPong(pTile new_tile, uint16_t discard_player_index) {
  TileId new_tile_id = new_tile->GetId();

  auto hands = hand_->GetHands();
  auto iter1 = hands.end();
  auto iter2 = hands.end();

  for (auto iter = hands.begin(); iter != hands.end(); iter++) {
    TileId id = (*iter)->GetId();
    if (id == new_tile_id) {
      if (iter1 == hands.end()) {
        iter1 = iter;
      } else if (iter2 == hands.end()) {
        iter2 = iter;
      } else {
        break;
      }
    }
  }
  if (iter2 == hands.end()) {
    return false;
  }

  char option = 'n';
  std::cout << "You Can Do \"Pong\" Operation. [y/n]";
  std::cin >> option;
  while (option != 'y' && option != 'n') {
    std::cout << "Wrong Input, Please Try Again. [y/n]";
    std::cin >> option;
  }
  if (option == 'n') {
    return false;
  }

  uint16_t diff = (discard_player_index - player_index_ + 4) % 4;
  switch (diff) {
    case 1:
      expose_->AddNewMeld(Meld(MeldType::Triplet, *iter1, *iter2, new_tile));
      break;
    case 2:
      expose_->AddNewMeld(Meld(MeldType::Triplet, *iter1, new_tile, *iter2));
      break;
    case 3:
      expose_->AddNewMeld(Meld(MeldType::Triplet, new_tile, *iter1, *iter2));
      break;
    default:
      return false;
  }

  hand_->RemoveTile(iter1);
  hand_->RemoveTile(iter2);
  return true;
}

bool PlayerTileManager::TryKong(pTile new_tile, uint16_t discard_player_index) {
  TileId new_tile_id = new_tile->GetId();

  auto hands = hand_->GetHands();
  auto iter1 = hands.end();
  auto iter2 = hands.end();
  auto iter3 = hands.end();
  auto iter4 = hands.end();  // for concealed kong

  for (auto iter = hands.begin(); iter != hands.end(); iter++) {
    TileId id = (*iter)->GetId();
    if (id == new_tile_id) {
      if (iter1 == hands.end()) {
        iter1 = iter;
      } else if (iter2 == hands.end()) {
        iter2 = iter;
      } else if (iter3 == hands.end()) {
        iter3 = iter;
      } else {
        if (discard_player_index == player_index_ && iter4 == hands.end()) {
          iter4 = iter;
        }
        break;
      }
    }
  }
  if (iter3 == hands.end()
      || (discard_player_index == player_index_ && iter4 == hands.end())) {
    return false;
  }

  char option = 'n';
  std::cout << "You Can Do \"Kong\" Operation. [y/n]";
  std::cin >> option;
  while (option != 'y' && option != 'n') {
    std::cout << "Wrong Input, Please Try Again. [y/n]";
    std::cin >> option;
  }
  if (option == 'n') {
    return false;
  }

  uint16_t diff = (discard_player_index - player_index_ + 4) % 4;
  switch (diff) {
    case 0:
      expose_->AddNewMeld(
          Meld(MeldType::ConcealedKong, *iter1, *iter2, *iter3, *iter4));
      break;
    case 1:
      expose_->AddNewMeld(
          Meld(MeldType::ExposeKong, *iter1, *iter2, *iter3, new_tile));
      break;
    case 2:
      expose_->AddNewMeld(
          Meld(MeldType::ExposeKong, *iter1, new_tile, *iter2, *iter3));
      break;
    case 3:
      expose_->AddNewMeld(
          Meld(MeldType::ExposeKong, new_tile, *iter1, *iter2, *iter3));
      break;
    default:
      return false;
  }

  hand_->RemoveTile(iter1);
  hand_->RemoveTile(iter2);
  hand_->RemoveTile(iter3);
  if (iter4 != hands.end()) {
    hand_->RemoveTile(iter4);
  }
  return true;
}

MatchResult PlayerTileManager::TryWin(pTile new_tile) {
  if (MatchResult res = YakusMatcher::TryAllYakuMatch(
          *hand_, *expose_, new_tile, in_riichi_, false);
      res.HasResult()) {
    char option = 0;
    std::cout << "\nYou Can Win, Do you? [y/n]";
    std::cin >> option;
    while (option != 'y' && option != 'n') {
      std::cout << "Wrong Input, Please Try Again. [y/n]";
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
  expose_->Show();
}
};  // namespace mahjong