#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "TileTypes.hpp"

using pTile = std::unique_ptr<class Tile>;
using tile_id_t = uint16_t;

class Tile {
 public:
  virtual ~Tile() = default;

  static pTile from_string(const std::string& str);
  static pTile from_id(tile_id_t id);
  static tile_id_t transform_string2id(const std::string& str);
  static std::string transform_id2string(tile_id_t id);
  std::string get_ANSI();
  virtual std::string to_string() = 0;
  virtual tile_id_t to_id() = 0;

  TileCategory get_category() { return category_; }
  void set_owner(uint16_t index) { owner_index_ = index; }

 protected:
  TileCategory category_;
  uint16_t owner_index_ = -1;  // -1 表示无主牌, 0~3 for 东南西北
};

class SuitTile : public Tile {
 public:
  explicit SuitTile(SuitType type, uint16_t rank);
  virtual std::string to_string() noexcept;
  virtual tile_id_t to_id() noexcept;

 private:
  uint16_t rank_;  // 点数
  SuitType type_;
};

class HonorTile : public Tile {
 public:
  explicit HonorTile(HonorType type);
  virtual std::string to_string() noexcept;
  virtual tile_id_t to_id() noexcept;

 private:
  HonorType type_;
};
