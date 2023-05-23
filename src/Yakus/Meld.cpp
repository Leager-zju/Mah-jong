#include "Yakus/Meld.hpp"

#include "Tiles.hpp"

#include <iostream>
#include <string>

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
  std::cout << ToString();
  std::cout << '\n';
}

std::string Meld::ToString() const {
  std::string res;
  for (auto&& tile : meld_) {
    res += tile->ToString();
  }
  return res;
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