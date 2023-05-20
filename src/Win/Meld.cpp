#include "Meld.hpp"

#include <iostream>

#include "Tiles.hpp"

void Meld::show() const {
  switch (meld_type_) {
    case MeldType::Eyes:
      std::cout << "Eyes: ";
      break;
    case MeldType::Triplet:
      std::cout << "Triplet: ";
      break;
    case MeldType::Sequence:
      std::cout << "Sequence: ";
      break;
    case MeldType::ExposeKong:
      std::cout << "ExposeKong: ";
      break;
    case MeldType::ConcealedKong:
      std::cout << "ConcealedKong: ";
      break;
  }
  for (auto&& tile : meld_) {
    std::cout << tile->to_string();
  }
  std::cout << '\n';
}

void MeldInId::show() const {
  switch (meld_type_) {
    case MeldType::Eyes:
      std::cout << "Eyes: ";
      break;
    case MeldType::Triplet:
      std::cout << "Triplet: ";
      break;
    case MeldType::Sequence:
      std::cout << "Sequence: ";
      break;
    case MeldType::ExposeKong:
      std::cout << "ExposeKong: ";
      break;
    case MeldType::ConcealedKong:
      std::cout << "ConcealedKong: ";
      break;
  }
  for (auto&& id : tile_ids_) {
    std::cout << Tile::transform_id2string(id);
  }
  std::cout << '\n';
}