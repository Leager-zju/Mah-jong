#include "PlayerTileManager.hpp"

#include "Common.hpp"
#include "DoraYaku.hpp"
#include "Global.hpp"
#include "GlobalTileManager.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "RiichiDetector.hpp"
#include "Tiles.hpp"
#include "YakusMatcher.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace mahjong {
void PlayerTileManager::Initial() {
  hand_->Initial();
  expose_->Initial();
  discards_.clear();
  in_riichi_ = false;
}
void PlayerTileManager::Draw(pTile new_tile) {
  if (!new_tile) {
    return;
  }
  new_tile->SetOwner(player_index_);
  if (is_my_player_) {
    std::cout << "\nYou Draw Tile: " << new_tile->ToString() << '\n';
  }
  hand_->Draw(new_tile);
}

void PlayerTileManager::ReceiveDiscardTile(pTile discard_tile) {
  discards_.push_back(discard_tile);
}

pTile PlayerTileManager::Discard() const {
  pTile discard_tile = nullptr;
  if (is_my_player_ && !in_riichi_) {
    std::string header = "Input the tile you want to discard [default: "
                         + hand_->GetLastDraw()->ToString() + "]";
    global::GetInput(header.c_str(),
                     [&discard_tile, this](const std::string& input) -> bool {
                       return hand_->Discard(input, discard_tile);
                     });

  } else {
    // discard_tile = hand_->RandomDiscard();
    discard_tile = hand_->DiscardLastDraw();
  }
  return discard_tile;
}

WinningResult PlayerTileManager::TryWin(pTile new_tile, bool self_drawn) {
  if (!new_tile) {
    return {};
  }
  WinningResult res = mahjong::yakus_matcher::TryAllYakuMatch(
      *hand_, *expose_, new_tile, in_riichi_, self_drawn);
  if (res.HasResult() && is_my_player_) {
    if (char option = global::GetInput("You Can Win. [y/n]",
                                       [](const std::string& input) -> bool {
                                         return input[0] == 'y'
                                                || input[0] == 'n';
                                       })[0];
        option == 'y') {
      Dora::TryMatch(*hand_, *expose_, in_riichi_, res);
    }
  }
  return res;
}

pTile PlayerTileManager::TryRiichi() {
  pTile discard_tile = nullptr;
  if (in_riichi_) {
    return discard_tile;
  }
  auto waiting_results = riichi_detector::CheckRiichi(*hand_, *expose_);
  if (waiting_results.empty()) {
    return discard_tile;
  }

  std::string header
      = "You can riichi. Tiles you can discard are show below, please make a "
        "choice.\n0. Cancel\n";
  for (size_t i = 0; i < waiting_results.size(); i++) {
    std::string choice = std::to_string(i + 1) + ". ";
    choice += Tile::TransformId2string(waiting_results[i].GetDiscard());
    choice += "  WaitFor: ";
    for (auto&& tile_id : waiting_results[i].GetWaitingFor()) {
      choice.push_back(' ');
      choice += Tile::TransformId2string(tile_id);
    }
    choice.push_back('\n');
    header += choice;
  }
  std::string option = global::GetInput(
      header.c_str(), [&waiting_results](const std::string& input) -> bool {
        if (input.empty()) {
          return false;
        }
        int index = std::stoi(input);
        return index >= 0 && index <= waiting_results.size();
      });

  int index = std::stoi(option);
  if (index == 0) {
    return discard_tile;
  }

  TileId discard_tile_id = waiting_results[index - 1].GetDiscard();
  hand_->Discard(Tile::TransformId2string(discard_tile_id), discard_tile);
  in_riichi_ = true;
  return discard_tile;
}

bool PlayerTileManager::TryChi(pTile new_tile) {
  if (!new_tile || in_riichi_) {
    return false;
  }
  if (Tile::IsHonor(new_tile->GetId())) {
    return false;
  }

  TileId new_tile_id = new_tile->GetId();
  auto&& in_hand       = hand_->GetHands();
  std::vector<std::vector<Tile_Iter>> iters;
  std::vector<Meld> possible_melds;

  uint16_t rank = new_tile_id % 10;
  if (rank <= 7) {
    auto plus1 = static_cast<TileId>(new_tile_id + 1);
    auto plus2 = static_cast<TileId>(new_tile_id + 2);
    auto iter1 = hand_->FindTile(plus1);
    auto iter2 = hand_->FindTile(plus2);
    if (iter1 != in_hand.end() && iter2 != in_hand.end()) {
      iters.push_back({iter1, iter2});
      possible_melds.emplace_back(MeldType::Sequence, new_tile, *iter1, *iter2);
    }
  }
  if (rank >= 2 && rank <= 8) {
    auto minus1 = static_cast<TileId>(new_tile_id - 1);
    auto plus1  = static_cast<TileId>(new_tile_id + 1);
    auto iter1  = hand_->FindTile(minus1);
    auto iter2  = hand_->FindTile(plus1);
    if (iter1 != in_hand.end() && iter2 != in_hand.end()) {
      iters.push_back({iter1, iter2});
      possible_melds.emplace_back(MeldType::Sequence, *iter1, new_tile, *iter2);
    }
  }
  if (rank >= 3) {
    auto minus2 = static_cast<TileId>(new_tile_id - 2);
    auto minus1 = static_cast<TileId>(new_tile_id - 1);
    auto iter1  = hand_->FindTile(minus2);
    auto iter2  = hand_->FindTile(minus1);
    if (iter1 != in_hand.end() && iter2 != in_hand.end()) {
      iters.push_back({iter1, iter2});
      possible_melds.emplace_back(MeldType::Sequence, *iter1, *iter2, new_tile);
    }
  }
  if (possible_melds.empty()) {
    return false;
  }

  std::cout << "You Can Do \"Chi\" Operation:\n";
  std::cout << "0. Cancel\n";
  for (auto i = 0; i < possible_melds.size(); i++) {
    std::cout << std::to_string(i + 1) << ". " << possible_melds[i].ToString()
              << '\n';
  }

  char input
      = global::GetInput("Choose A Meld You Want To Chi.",
                         [&possible_melds](const std::string& input) -> bool {
                           uint16_t index = input[0] - '0';
                           return index <= possible_melds.size();
                         })[0];
  uint16_t select_index = input - '0';
  if (select_index == 0) {
    return false;
  }

  hand_->RemoveTiles(iters[select_index - 1]);
  expose_->AddNewMeld(possible_melds[select_index - 1]);
  return true;
}

bool PlayerTileManager::TryPong(pTile new_tile, uint16_t discard_player_index) {
  if (!new_tile || in_riichi_) {
    return false;
  }

  TileId new_tile_id = new_tile->GetId();
  auto&& tile_iters  = hand_->FindTile(new_tile_id, 2);

  if (tile_iters.size() < 2) {
    return false;
  }

  if (global::GetInput("You Can Do \"Pong\" Operation. [y/n]",
                       [](const std::string& input) -> bool {
                         return input[0] == 'y' || input[0] == 'n';
                       })[0]
      == 'n') {
    return false;
  }

  uint16_t diff = (discard_player_index - player_index_ + 4) % 4;
  switch (diff) {
    case 1:
      expose_->AddNewMeld(
          Meld(MeldType::Triplet, *tile_iters[0], *tile_iters[1], new_tile));
      break;
    case 2:
      expose_->AddNewMeld(
          Meld(MeldType::Triplet, *tile_iters[0], new_tile, *tile_iters[1]));
      break;
    case 3:
      expose_->AddNewMeld(
          Meld(MeldType::Triplet, new_tile, *tile_iters[0], *tile_iters[1]));
      break;
    default:
      return false;
  }
  hand_->RemoveTiles(tile_iters);
  return true;
}

bool PlayerTileManager::TryKong(pTile new_tile, uint16_t discard_player_index) {
  bool concealed_kong = discard_player_index == player_index_;
  if (!new_tile || (in_riichi_ && !concealed_kong)) {
    return false;
  }
  if (GlobalTileManager::GetTileManager()->DeadEmpty()) {
    return false;
  }

  TileId new_tile_id = new_tile->GetId();
  auto num           = concealed_kong ? 4 : 3;
  auto&& tile_iters  = hand_->FindTile(new_tile_id, num);

  if (tile_iters.size() < num) {
    return false;
  }

  if (char option = global::GetInput("You Can Do \"Kong\" Operation. [y/n]",
                                     [](const std::string& input) -> bool {
                                       return input[0] == 'y'
                                              || input[0] == 'n';
                                     })[0];
      option == 'n') {
    return false;
  }

  uint16_t diff = (discard_player_index - player_index_ + 4) % 4;
  switch (diff) {
    case 0:
      expose_->AddNewMeld(Meld(MeldType::ConcealedKong,
                               *tile_iters[0],
                               *tile_iters[1],
                               *tile_iters[2],
                               *tile_iters[3]));
      break;
    case 1:
      expose_->AddNewMeld(Meld(MeldType::ExposeKong,
                               *tile_iters[0],
                               *tile_iters[1],
                               *tile_iters[2],
                               new_tile));
      break;
    case 2:
      expose_->AddNewMeld(Meld(MeldType::ExposeKong,
                               *tile_iters[0],
                               new_tile,
                               *tile_iters[1],
                               *tile_iters[2]));
      break;
    case 3:
      expose_->AddNewMeld(Meld(MeldType::ExposeKong,
                               new_tile,
                               *tile_iters[0],
                               *tile_iters[1],
                               *tile_iters[2]));
      break;
    default:
      return false;
  }

  hand_->RemoveTiles(tile_iters);
  return true;
}

void PlayerTileManager::ShowHand() const {
  hand_->Show();
}

void PlayerTileManager::ShowExpose() const {
  expose_->Show();
}

void PlayerTileManager::ShowDiscard() const {
  for (auto&& tile : discards_) {
    std::cout << tile->ToString() << " ";
  }
  std::cout << '\n';
}
};  // namespace mahjong