#include "GlobalTileManager.hpp"

#include "Common.hpp"
#include "Tiles.hpp"

#include <corecrt.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

namespace mahjong {
std::array<Tile, TOTAL> tile_pool = {
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

    Tile(E),   Tile(E),   Tile(E),   Tile(E),   Tile(S),   Tile(S),   Tile(S),
    Tile(S),   Tile(W),   Tile(W),   Tile(W),   Tile(W),   Tile(N),   Tile(N),
    Tile(N),   Tile(N),   Tile(Z),   Tile(Z),   Tile(Z),   Tile(Z),   Tile(F),
    Tile(F),   Tile(F),   Tile(F),   Tile(B),   Tile(B),   Tile(B),   Tile(B)};

const std::unique_ptr<GlobalTileManager> GlobalTileManager::GLOBAL_TILE_MANAGER
    = std::make_unique<GlobalTileManager>();

GlobalTileManager::GlobalTileManager() {
  deck_.resize(TOTAL);
  disacrd_pile_.resize(TOTAL);
}

const std::unique_ptr<GlobalTileManager>& GlobalTileManager::GetTileManager() {
  return GLOBAL_TILE_MANAGER;
}

void GlobalTileManager::Initial() {
  // 清理所有 pTile
  disacrd_pile_.clear();

  int index = 0;
  for (auto&& tile : tile_pool) {
    tile.Initial();
    deck_[index++] = &tile;
  }
  Shuffle();
}

pTile GlobalTileManager::Pop() {
  if (head_ > TILE_UPPER_BOUND) {
    return nullptr;
  }

  return deck_[head_++];
}

void GlobalTileManager::ReceiveDiscardTile(pTile discard_tile) {
  disacrd_pile_.emplace_back(discard_tile);
}

void GlobalTileManager::Shuffle() {
  std::random_device rand;
  std::mt19937 random_generator(rand());
  std::shuffle(deck_.begin(), deck_.end(), random_generator);
}

void GlobalTileManager::ShowDeck() const {
  std::cout << "Deck: ";
  for (uint16_t i = head_; i < tail_; i++) {
    std::cout << deck_[i]->ToString() << " ";
  }
  std::cout << '\n';
}

void GlobalTileManager::ShowDiscardPile() const {
  std::cout << "Discard Pile: ";
  for (auto&& tile : disacrd_pile_) {
    std::cout << tile->ToString() << " ";
  }
  std::cout << '\n';
}

void GlobalTileManager::ShowDoraIndicator() const {
  std::cout << "Dora Indicator: ";
  for (uint16_t i  = DORA_TILE_UPPER_BOUND - 1; i >= DORA_TILE_LOWER_BOUND;
       i          -= 2) {
    if (i >= dora_) {
      std::cout << deck_[i]->ToString() << " ";
    } else {
      std::cout << "\033[31m \033[0m ";
    }
  }
  std::cout << '\n';
}

bool GlobalTileManager::IsDora(TileId id) const {
  for (auto i = DORA_TILE_UPPER_BOUND - 1; i >= dora_; i -= 2) {
    if (id == Tile::Next(deck_[i]->ToId())) {
      return true;
    }
  }
  return false;
}

bool GlobalTileManager::IsInnerDora(TileId id) const {
  for (auto i = DORA_TILE_UPPER_BOUND; i >= inner_dora_; i -= 2) {
    if (id == Tile::Next(deck_[i]->ToId())) {
      return true;
    }
  }
  return false;
}
};  // namespace mahjong