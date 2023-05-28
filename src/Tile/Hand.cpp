#include "Hand.hpp"

#include "Common.hpp"
#include "Tiles.hpp"

#include <corecrt.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <random>
#include <vector>

namespace mahjong {
void Hand::Draw(pTile new_tile) {
  if (!new_tile) {
    return;
  }
  auto&& iter = in_hand_.begin();
  while (iter != in_hand_.end()) {
    if ((*iter)->GetId() >= new_tile->GetId()) {
      break;
    }
    iter++;
  }
  last_draw_ = in_hand_.emplace(iter, new_tile);
}

bool Hand::Discard(const std::string& discard_string, pTile& discard_tile) {
  if (discard_string.empty()) {
    discard_tile = *last_draw_;
    in_hand_.erase(last_draw_);
    return true;
  }
  TileId id    = Tile::TransformString2id(discard_string);
  auto&& iters = FindTile(id, 1);
  if (iters.empty()) {
    return false;
  }
  discard_tile = *iters[0];
  in_hand_.erase(iters[0]);
  return true;
}

pTile Hand::RandomDiscard() {
  std::random_device rand;
  std::mt19937 mt(rand());
  std::uniform_int_distribution<size_t> dist(0, in_hand_.size() - 1);
  size_t rand_index = dist(mt);

  auto&& iter = in_hand_.begin();
  for (size_t i = 0; i < rand_index; i++) {
    iter++;
  }
  pTile res = *iter;
  in_hand_.erase(iter);
  return res;
}

pTile Hand::DiscardLastDraw() {
  pTile res = *last_draw_;
  in_hand_.erase(last_draw_);
  return res;
}

void Hand::Show() const {
  for (auto&& tile : in_hand_) {
    std::cout << tile->ToString() << " ";
  }
  std::cout << '\n';
}

void Hand::RemoveTile(TileId id, uint16_t count) {
  RemoveTiles(FindTile(id, count));
}

void Hand::RemoveTiles(const std::vector<Tile_Iter>& iters) {
  for (auto iter : iters) {
    in_hand_.erase(iter);
  }
}

Tile_Iter Hand::FindTile(TileId id) {
  auto iter = in_hand_.begin();
  while (iter != in_hand_.end()) {
    if ((*iter)->GetId() == id) {
      break;
    }
    iter++;
  }
  return iter;
}

std::vector<Tile_Iter> Hand::FindTile(TileId id, uint16_t count) {
  std::vector<Tile_Iter> res;
  for (auto iter = in_hand_.begin(); iter != in_hand_.end(); iter++) {
    if ((*iter)->GetId() == id && count--) {
      res.push_back(iter);
    }
  }
  return res;
}
};  // namespace mahjong