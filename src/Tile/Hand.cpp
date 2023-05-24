#include "Hand.hpp"

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

namespace mahjong {
void Hand::Draw(const pTile& new_tile) {
  auto&& iter = in_hand_.begin();
  while (iter != in_hand_.end()) {
    if ((*iter)->GetId() >= new_tile->GetId()) {
      break;
    }
    iter++;
  }
  in_hand_.emplace(iter, new_tile);
}

bool Hand::Discard(const std::string& discard_string, pTile& discard_tile) {
  TileId id = Tile::TransformString2id(discard_string);
  std::cout << id << " ";
  for (auto&& tile : in_hand_) {
    std::cout << tile->GetId() << ' ';
  }
  for (auto&& iter = in_hand_.begin(); iter != in_hand_.end(); iter++) {
    if ((*iter)->GetId() == id) {
      discard_tile = *iter;
      RemoveTile(iter);
      return true;
    }
  }
  return false;
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

void Hand::Show() const {
  for (auto&& tile : in_hand_) {
    std::cout << tile->ToString() << " ";
  }
  std::cout << '\n';
}
};  // namespace mahjong