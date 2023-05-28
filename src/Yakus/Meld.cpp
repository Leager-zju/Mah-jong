#include "Yakus/Meld.hpp"

#include "Tiles.hpp"

#include <iostream>
#include <string>

namespace mahjong {
void Meld::Show() const {
  std::cout << ToString() << " ";
}

std::string Meld::ToString() const {
  std::string res;
  for (auto&& tile : meld_) {
    res += tile->ToString();
  }
  return res;
}

void MeldInId::Show() const {
  for (auto&& id : tile_ids_) {
    std::cout << Tile::TransformId2string(id);
  }
  std::cout << '\n';
}
};  // namespace mahjong