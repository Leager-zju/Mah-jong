#pragma once

#include <iostream>
#include <vector>

#include "Meld.hpp"
#include "Tiles.hpp"

/**
 * 副露区
 */
class Expose {
 public:
  Expose() = default;
  explicit Expose(std::vector<Meld>& in_expose) : in_expose_(in_expose) {}
  void add_new_meld(const Meld& meld) { in_expose_.emplace_back(meld); }
  void show() const {
    std::cout << "Expose:";
    for (auto&& meld : in_expose_) {
      meld.show();
    }
    if (in_expose_.empty()) {
      std::cout << '\n';
    }
  }
  const std::vector<Meld>& get_exposes() const { return in_expose_; }

 private:
  std::vector<Meld> in_expose_;
};