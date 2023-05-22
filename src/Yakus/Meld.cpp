#include "Yakus/Meld.hpp"

#include "Tiles.hpp"

#include <iostream>

namespace mahjong {
void Meld::Show() const {
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
    std::cout << tile->ToString();
  }
  std::cout << '\n';
}

void MeldInId::Show() const {
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
    std::cout << Tile::TransformId2string(id);
  }
  std::cout << '\n';
}
};  // namespace mahjong