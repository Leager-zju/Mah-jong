#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "TileTypes.hpp"

using pTile = class Tile*;

class Tile {
 public:
  explicit Tile(tile_id id) : id_(id) {}
  explicit Tile(const std::string& str) : id_(transform_string2id(str)) {}
  static tile_id transform_string2id(const std::string& str);
  static std::string transform_id2string(tile_id id);

  void initial();
  std::string get_ANSI();
  std::string to_string() { return get_ANSI() + transform_id2string(id_) + "\033[0m"; }
  tile_id to_id() { return id_; }
  void become_dora() { is_dora = true; }
  void set_owner(uint8_t index) { owner_index_ = index; }

 protected:
  bool is_dora = false;
  uint8_t owner_index_ = -1;  // -1 for 无主牌, 0~3 for 东南西北
  tile_id id_ = NAT;
};
