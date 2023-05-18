#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "TileTypes.hpp"

class Tile {
 public:
  static std::shared_ptr<Tile> from_string(std::string&& str);
  static std::shared_ptr<Tile> from_id(uint8_t id);

  TileCategory get_category() { return category_; }
  virtual std::string to_string() = 0;
  virtual uint8_t to_id() = 0;

 protected:
  TileCategory category_;
};

class SuitTile : public Tile {
 public:
  explicit SuitTile(SuitType type, uint8_t rank);
  virtual std::string to_string() noexcept;
  virtual uint8_t to_id() noexcept;

 private:
  uint8_t rank_;  // 点数
  SuitType type_;
};

class HonorTile : public Tile {
 public:
  explicit HonorTile(HonorType type);
  virtual std::string to_string() noexcept;
  virtual uint8_t to_id() noexcept;

 private:
  HonorType type_;
};
