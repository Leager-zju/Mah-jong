#include "Meld.hpp"

#include <iostream>

#include "Tiles.hpp"

void Eyes::show() {
  std::cout << "Eyes: ";
  for (int i = 0; i < 2; i++) {
    std::cout << Tile::transform_id2string(tile_id_);
  }
  std::cout << '\n';
}

void Triplet::show() {
  std::cout << "Triplet: ";
  for (int i = 0; i < 3; i++) {
    std::cout << Tile::transform_id2string(tile_id_);
  }
  std::cout << '\n';
}

void Sequence::show() {
  std::cout << "Sequence: ";
  for (int i = 0; i < 3; i++) {
    std::cout << Tile::transform_id2string(tile_id_[i]);
  }
  std::cout << '\n';
}

void ExposeKong::show() {
  std::cout << "ExposeKong: ";
  for (int i = 0; i < 4; i++) {
    std::cout << Tile::transform_id2string(tile_id_);
  }
  std::cout << '\n';
}

void ConcealedKong::show() {
  std::cout << "ConcealedKong: ";
  for (int i = 0; i < 4; i++) {
    std::cout << Tile::transform_id2string(tile_id_);
  }
  std::cout << '\n';
}
