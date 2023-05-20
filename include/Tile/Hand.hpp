#pragma once

#include <list>
#include <optional>
#include <vector>

#include "Tiles.hpp"

/**
 * 手牌
 */
class Hand {
 public:
  Hand() = default;
  explicit Hand(const std::list<pTile>& in_hand) : in_hand_(in_hand) {}
  void draw(const pTile& new_tile);
  bool discard(const std::string& discard_string, pTile& discard_tile);
  pTile random_discard();
  void show() const;
  const std::list<pTile>& get_hands() const { return in_hand_; }
  // void check_concealed_kong(int& start_index);
  // bool try_concealed_kong(const int start_index);
  // std::vector<pTile> do_concealed_kong(const int start_index);

 private:
  std::list<pTile> in_hand_;
};