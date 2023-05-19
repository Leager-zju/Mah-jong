#pragma once

#include <vector>

#include "Tiles.hpp"

/**
 * 副露区
 */
class Expose {
 public:
  void add_triplet(std::vector<pTile>&& triplet);
  void add_expose_kong(std::vector<pTile>&& expose_kong);
  void add_concealed_kong(std::vector<pTile>&& concealed_kong);
  void add_sequence(std::vector<pTile>&& sequence);
  void show();

 private:
  std::vector<std::vector<pTile>> in_expose_;
};