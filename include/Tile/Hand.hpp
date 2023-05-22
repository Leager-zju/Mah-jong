#pragma once

#include "Tiles.hpp"

#include <list>
#include <optional>
#include <vector>

namespace mahjong {
/**
 * 手牌
 */
class Hand {
 public:
  Hand() = default;
  explicit Hand(const std::list<pTile>& in_hand) : in_hand_(in_hand) {}
  void Draw(const pTile& new_tile);
  bool Discard(const std::string& discard_string, pTile& discard_tile);
  pTile RandomDiscard();
  void Show() const;
  const std::list<pTile>& GetHands() const {
    return in_hand_;
  }
  // void check_concealed_kong(int& start_index);
  // bool try_concealed_kong(const int start_index);
  // std::vector<pTile> do_concealed_kong(const int start_index);

 private:
  std::list<pTile> in_hand_;
};
};  // namespace mahjong