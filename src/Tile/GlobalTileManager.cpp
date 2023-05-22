#include "GlobalTileManager.hpp"

#include <corecrt.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "Common.hpp"
#include "Tiles.hpp"

namespace MAHJONG {
static Tile tile_pool[136] = {
    Tile(_1m), Tile(_1m), Tile(_1m), Tile(_1m), Tile(_2m), Tile(_2m), Tile(_2m),
    Tile(_2m), Tile(_3m), Tile(_3m), Tile(_3m), Tile(_3m), Tile(_4m), Tile(_4m),
    Tile(_4m), Tile(_4m), Tile(_5m), Tile(_5m), Tile(_5m), Tile(_5m), Tile(_6m),
    Tile(_6m), Tile(_6m), Tile(_6m), Tile(_7m), Tile(_7m), Tile(_7m), Tile(_7m),
    Tile(_8m), Tile(_8m), Tile(_8m), Tile(_8m), Tile(_9m), Tile(_9m), Tile(_9m),
    Tile(_9m),

    Tile(_1p), Tile(_1p), Tile(_1p), Tile(_1p), Tile(_2p), Tile(_2p), Tile(_2p),
    Tile(_2p), Tile(_3p), Tile(_3p), Tile(_3p), Tile(_3p), Tile(_4p), Tile(_4p),
    Tile(_4p), Tile(_4p), Tile(_5p), Tile(_5p), Tile(_5p), Tile(_5p), Tile(_6p),
    Tile(_6p), Tile(_6p), Tile(_6p), Tile(_7p), Tile(_7p), Tile(_7p), Tile(_7p),
    Tile(_8p), Tile(_8p), Tile(_8p), Tile(_8p), Tile(_9p), Tile(_9p), Tile(_9p),
    Tile(_9p),

    Tile(_1s), Tile(_1s), Tile(_1s), Tile(_1s), Tile(_2s), Tile(_2s), Tile(_2s),
    Tile(_2s), Tile(_3s), Tile(_3s), Tile(_3s), Tile(_3s), Tile(_4s), Tile(_4s),
    Tile(_4s), Tile(_4s), Tile(_5s), Tile(_5s), Tile(_5s), Tile(_5s), Tile(_6s),
    Tile(_6s), Tile(_6s), Tile(_6s), Tile(_7s), Tile(_7s), Tile(_7s), Tile(_7s),
    Tile(_8s), Tile(_8s), Tile(_8s), Tile(_8s), Tile(_9s), Tile(_9s), Tile(_9s),
    Tile(_9s),

    Tile(_E),  Tile(_E),  Tile(_E),  Tile(_E),  Tile(_S),  Tile(_S),  Tile(_S),
    Tile(_S),  Tile(_W),  Tile(_W),  Tile(_W),  Tile(_W),  Tile(_N),  Tile(_N),
    Tile(_N),  Tile(_N),  Tile(_Z),  Tile(_Z),  Tile(_Z),  Tile(_Z),  Tile(_F),
    Tile(_F),  Tile(_F),  Tile(_F),  Tile(_B),  Tile(_B),  Tile(_B),  Tile(_B)};

std::unique_ptr<GlobalTileManager> GlobalTileManager::global_tile_manager_ =
    nullptr;

GlobalTileManager::GlobalTileManager() {
  deck_.resize(TOTAL);
  disacrd_pile_.resize(TOTAL);
}

std::unique_ptr<GlobalTileManager>& GlobalTileManager::get_tile_manager() {
  if (!global_tile_manager_.get()) {
    global_tile_manager_ = std::make_unique<GlobalTileManager>();
  }
  return global_tile_manager_;
}

void GlobalTileManager::initial() {
  // 清理所有 pTile
  disacrd_pile_.clear();

  for (size_t i = 0; i < TOTAL; i++) {
    tile_pool[i].initial();
    deck_[i] = tile_pool + i;
  }
  shuffle();
}

pTile GlobalTileManager::pop() {
  if (head_ > TILE_UPPER_BOUND) {
    return nullptr;
  }

  return deck_[head_++];
}

void GlobalTileManager::receive_discard_tile(pTile discard_tile) {
  disacrd_pile_.emplace_back(discard_tile);
}

void GlobalTileManager::shuffle() {
  std::random_device rd;
  std::mt19937 random_generator(rd());
  std::shuffle(deck_.begin(), deck_.end(), random_generator);
}

void GlobalTileManager::show_deck() {
  std::cout << "Deck: ";
  for (uint16_t i = head_; i < tail_; i++) {
    std::cout << deck_[i]->to_string() << " ";
  }
  std::cout << '\n';
}

void GlobalTileManager::show_discard_pile() {
  std::cout << "Discard Pile: ";
  for (size_t i = 0; i < disacrd_pile_.size(); i++) {
    std::cout << disacrd_pile_[i]->to_string() << " ";
  }
  std::cout << '\n';
}

void GlobalTileManager::show_dora_indicator() {
  std::cout << "Dora Indicator: ";
  for (uint16_t i = DORA_TILE_UPPER_BOUND - 1; i >= DORA_TILE_LOWER_BOUND;
       i -= 2) {
    if (i >= dora_) {
      std::cout << deck_[i]->to_string() << " ";
    } else {
      // std::cout << "\033[31m \033[0m ";
    }
  }
  std::cout << '\n';
}

bool GlobalTileManager::is_dora(tile_id id) {
  for (auto i = DORA_TILE_UPPER_BOUND - 1; i >= dora_; i -= 2) {
    if (id == Tile::next(deck_[i]->to_id())) {
      return true;
    }
  }
  return false;
}

bool GlobalTileManager::is_inner_dora(tile_id id) {
  for (auto i = DORA_TILE_UPPER_BOUND; i >= inner_dora_; i -= 2) {
    if (id == Tile::next(deck_[i]->to_id())) {
      return true;
    }
  }
  return false;
}
};  // namespace MAHJONG