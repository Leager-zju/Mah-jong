#pragma once

#include "Meld.hpp"

#include <iostream>
#include <vector>

namespace mahjong {
/**
 * 副露区
 */
class Expose {
 public:
  Expose() = default;
  explicit Expose(std::vector<Meld>& in_expose) : in_expose_(in_expose) {}
  void AddNewMeld(const Meld& meld);
  void Show() const;
  const std::vector<Meld>& GetExposes() const {
    return in_expose_;
  }

 private:
  std::vector<Meld> in_expose_;
};
};  // namespace mahjong