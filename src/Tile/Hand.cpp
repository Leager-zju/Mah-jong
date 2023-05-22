#include "Hand.hpp"

#include <corecrt.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <optional>

#include "Tiles.hpp"

namespace MAHJONG {
void Hand::draw(const pTile& new_tile) {
  auto&& iter = in_hand_.begin();
  while (iter != in_hand_.end()) {
    if ((*iter)->to_id() >= new_tile->to_id()) {
      break;
    }
    iter++;
  }
  in_hand_.emplace(iter, new_tile);
}

bool Hand::discard(const std::string& discard_string, pTile& discard_tile) {
  tile_id id = Tile::transform_string2id(discard_string);
  for (auto&& iter = in_hand_.begin(); iter != in_hand_.end(); iter++) {
    if ((*iter)->to_id() == id) {
      discard_tile = *iter;
      in_hand_.erase(iter);
      return true;
    }
  }
  return false;
}

pTile Hand::random_discard() {
  size_t rd = std::rand() % in_hand_.size();
  auto&& iter = in_hand_.begin();
  for (size_t i = 0; i < rd; i++) {
    iter++;
  }
  pTile res = *iter;
  in_hand_.erase(iter);
  return res;
}

void Hand::show() const {
  std::cout << "Hands: ";
  for (auto&& tile : in_hand_) {
    std::cout << tile->to_string() << " ";
  }
  std::cout << '\n';
}

// void Hand::check_concealed_kong(int& start_index) {
//   uint16_t id = in_hand_[0]->to_id();
//   uint16_t count = 1;
//   for (auto&& i = 1; i < in_hand_.size(); i++) {
//     if (in_hand_[i]->to_id() == id) {
//       if (++count == 4) {
//         start_index = i - 3;
//         return;
//       }
//     } else {
//       id = in_hand_[i]->to_id();
//       count = 1;
//     }
//   }
// }

// bool Hand::try_concealed_kong(const int start_index) {
//   char response;
//   std::cout << "Kong " << in_hand_[start_index]->to_string() << " ?[y/n]";
//   std::cin >> response;
//   assert(response == 'y' || response == 'n');
//   return response == 'y';
// }

// std::vector<pTile> Hand::do_concealed_kong(const int start_index) {
//   std::vector<pTile> res;
//   res.reserve(4);
//   for (int i = 0; i < 4; i++) {
//     res.emplace_back(in_hand_[start_index]);
//     in_hand_.erase(in_hand_.begin() + start_index);
//   }
//   return res;
// }
};  // namespace MAHJONG