#include "Tiles.hpp"

#include "Common.hpp"
#include "global.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

namespace mahjong {
TileId Tile::TransformString2id(const std::string& str) {
  assert(str.length() >= 1 && str.length() <= 2);
  if (str.length() == 1) {
    switch (str[0]) {
      case 'E':
        return E;
      case 'S':
        return S;
      case 'W':
        return W;
      case 'N':
        return N;
      case 'Z':
        return Z;
      case 'B':
        return B;
      case 'F':
        return F;
      default:
        return NAT;
    }
  } else {
    uint16_t rank = str[0] - '0';
    switch (str[1]) {
      case 'm':
        return static_cast<TileId>(rank);
      case 'p':
        return static_cast<TileId>(PIN + rank);
      case 's':
        return static_cast<TileId>(SOU + rank);
      default:
        return NAT;
    }
  }
}

std::string Tile::TransformId2string(TileId id) {
  std::string res;
  uint16_t category = id / 10;
  uint16_t rank     = id % 10;

  switch (category) {
    case 0:
      return std::to_string(rank) + "m";
    case 1:
      return std::to_string(rank) + "p";
    case 2:
      return std::to_string(rank) + "s";
    case 3:
      return "E";
    case 4:
      return "S";
    case 5:
      return "W";
    case 6:
      return "N";
    case 7:
      return "Z";
    case 8:
      return "B";
    case 9:
      return "F";
    default:
      return "";
  }
}

std::string Tile::GetAnsi() const {
  return global::GetColor(owner_index_);
}

void Tile::Initial() {
  owner_index_ = -1;
}

TileId Tile::Next(TileId id) {
  if (!IsHonor(id)) {
    return id % 10 == 9 ? static_cast<TileId>(id - 8)
                        : static_cast<TileId>(id + 1);
  }
  if (IsWind(id)) {
    return id == N ? E : static_cast<TileId>(id + 10);
  }

  return id == F ? Z : static_cast<TileId>(id + 10);
}
};  // namespace mahjong