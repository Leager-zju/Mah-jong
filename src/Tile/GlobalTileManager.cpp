#include "GlobalTileManager.hpp"
#include <corecrt.h>

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "Tiles.hpp"

GlobalTileManager::GlobalTileManager() { deck_.reserve(TOTAL); disacrd_pile_.reserve(TOTAL); }

void GlobalTileManager::initial() {
  // 清理所有 pTile
  deck_.clear();
  disacrd_pile_.clear();

  for (uint16_t category = 0; category <= 2; category++) {
    for (uint16_t rank = 1; rank <= 9; rank++) {
      for (auto i = 0; i < 4; i++) {
        deck_.emplace_back(Tile::from_id(category * 10 + rank));
      }
    }
  }
  for (uint16_t category = 3; category <= 9; category++) {
    for (auto i = 0; i < 4; i++) {
      deck_.emplace_back(Tile::from_id(category * 10));
    }
  }
  shuffle();
}

pTile GlobalTileManager::pop() {
  if (head_ > TILE_UPPER_BOUND) {
    return nullptr;
  }

  return std::move(deck_[head_++]);
}

void GlobalTileManager::receive_discard_tile(pTile discard_tile) {
  disacrd_pile_.emplace_back(std::move(discard_tile));
}

void GlobalTileManager::shuffle() {
  std::random_device rd;
  std::mt19937 random_generator(rd());
  std::shuffle(deck_.begin(), deck_.end(), random_generator);
}

void GlobalTileManager::show_deck() {
  std::cout << "Deck:\n";
  for (auto i = head_; i < tail_; i++) {
    std::cout << deck_[i]->to_string() << " ";
  }
  std::cout << '\n';
}

void GlobalTileManager::show_discard_pile() {
  std::cout << "Discard Pile:\n";
  for (size_t i = 0; i < disacrd_pile_.size(); i++) {
    std::cout << disacrd_pile_[i]->to_string() << " ";
  }
  std::cout << '\n';
}