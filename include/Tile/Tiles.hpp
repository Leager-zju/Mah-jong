#pragma once

#include "Common.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace mahjong {
using pTile = class Tile*;

class Tile {
 public:
  explicit Tile(TileId id) : id_(id) {}
  explicit Tile(const std::string& str) : id_(TransformString2id(str)) {}
  static TileId TransformString2id(const std::string& str);
  static std::string TransformId2string(TileId id);

  static bool IsTerminal(TileId id) {
    return id == _1m || id == _9m || id == _1p || id == _9p || id == _1s
           || id == _9s || IsHonor(id);
  }
  static bool IsHonor(TileId id) {
    return IsWind(id) || IsDragon(id);
  }
  static bool IsWind(TileId id) {
    return id == E || id == S || id == W || id == N;
  }
  static bool IsDragon(TileId id) {
    return id == Z || id == B || id == F;
  }
  static TileId Next(TileId id);

  void Initial();
  std::string GetAnsi() const;
  std::string ToString() const {
    return GetAnsi() + TransformId2string(id_) + "\033[0m";
  }
  TileId GetId() const {
    return id_;
  }
  void SetOwner(uint8_t index) {
    owner_index_ = index;
  }

 private:
  uint8_t owner_index_ = -1;  // -1 for 无主牌, 0~3 for 东南西北
  TileId id_           = NAT;
};
};  // namespace mahjong
