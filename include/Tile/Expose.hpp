#pragma once

#include <iostream>
#include <vector>

#include "Meld.hpp"
#include "Tiles.hpp"

namespace MAHJONG {
/**
 * 副露区
 */
class Expose {
 public:
  Expose() = default;
  explicit Expose(std::vector<Meld>& in_expose) : in_expose_(in_expose) {}
  void add_new_meld(const Meld& meld);
  void show() const;
  const std::vector<Meld>& get_exposes() const { return in_expose_; }

 private:
  std::vector<Meld> in_expose_;
};
};  // namespace MAHJONG