#pragma once

#include <optional>
#include <vector>
#include <list>

#include "Tiles.hpp"

/**
 * 手牌
 */
class Hand {
 public:
  void draw(pTile new_tile);
  bool discard(const std::string& discard_string, pTile& discard_tile);
  pTile random_discard();
  void show();
  // void check_concealed_kong(int& start_index);
  // bool try_concealed_kong(const int start_index);
  // std::vector<pTile> do_concealed_kong(const int start_index);

 private:
  std::list<pTile> in_hand_;
};