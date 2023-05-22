#include "Tiles.hpp"

#include <assert.h>

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

#include "Common.hpp"

namespace MAHJONG {
tile_id Tile::transform_string2id(const std::string& str) {
  assert(!str.empty() && str.length() <= 2);
  if (str.length() == 1) {
    switch (str[0]) {
      case 'E':
        return _E;
      case 'S':
        return _S;
      case 'W':
        return _W;
      case 'N':
        return _N;
      case 'Z':
        return _Z;
      case 'B':
        return _B;
      case 'F':
        return _F;
      default:
        return NAT;
    }
  } else {
    uint16_t rank = str[0] - '0';
    switch (str[1]) {
      case 'm':
        return static_cast<tile_id>(rank);
      case 'p':
        return static_cast<tile_id>(10 + rank);
      case 's':
        return static_cast<tile_id>(20 + rank);
      default:
        return NAT;
    }
  }
}

std::string Tile::transform_id2string(tile_id id) {
  std::string res;
  uint16_t category = id / 10;
  uint16_t rank = id % 10;

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
      return "F";
    case 9:
      return "B";
    default:
      return "";
  }
}

std::string Tile::get_ANSI() {
  switch (owner_index_) {
    case 0:
      return "\033[31m";
    case 1:
      return "\033[32m";
    case 2:
      return "\033[33m";
    case 3:
      return "\033[34m";
  }

  return "";
}

void Tile::initial() {
  is_dora = false;
  owner_index_ = -1;
}

tile_id Tile::next(tile_id id) {
  if (!is_honor(id)) {
    return id % 10 == 9 ? static_cast<tile_id>(id - 8)
                        : static_cast<tile_id>(id + 1);
  }
  if (is_wind(id)) {
    return id == _N ? _E : static_cast<tile_id>(id + 10);
  }

  return id == _F ? _Z : static_cast<tile_id>(id + 10);
}
};  // namespace MAHJONG