#include "Tiles.hpp"

#include <assert.h>

#include <cstdint>
#include <memory>
#include <string>

#ifndef NDEBUG
#define NDEBUG

/**************************************
 *                Tile                *
 **************************************/
std::shared_ptr<Tile> Tile::from_string(std::string&& str) {
  assert(!str.empty() && str.length() <= 2);
  if (str.length() == 1) {
    switch (str[0]) {
      case 'E':
        return std::make_shared<HonorTile>(HonorType::E);
      case 'S':
        return std::make_shared<HonorTile>(HonorType::S);
      case 'W':
        return std::make_shared<HonorTile>(HonorType::W);
      case 'N':
        return std::make_shared<HonorTile>(HonorType::N);
      case 'Z':
        return std::make_shared<HonorTile>(HonorType::Z);
      case 'B':
        return std::make_shared<HonorTile>(HonorType::B);
      case 'F':
        return std::make_shared<HonorTile>(HonorType::F);
      default:
        return nullptr;
    }
  }

  uint8_t rank = str[0] - '0';
  switch (str[1]) {
    case 'm':
      return std::make_shared<SuitTile>(SuitType::m, rank);
    case 'p':
      return std::make_shared<SuitTile>(SuitType::p, rank);
    case 's':
      return std::make_shared<SuitTile>(SuitType::s, rank);
    default:
      return nullptr;
  }
}

std::shared_ptr<Tile> Tile::from_id(uint8_t id) {
  uint8_t type = id / 10;
  uint8_t rank = id % 10;
  if (type <= 2) {
    return std::make_shared<SuitTile>(static_cast<SuitType>(type), rank);
  }
  assert(type >= 3 && type <= 9 && rank == 0);
  return std::make_shared<HonorTile>(static_cast<HonorType>(type));
}

/**************************************
 *              SuitTile              *
 **************************************/
SuitTile::SuitTile(SuitType type, uint8_t rank) : type_(type), rank_(rank) { Tile::category_ = TileCategory::Suit; }

std::string SuitTile::to_string() noexcept {
  switch (type_) {
    case SuitType::m:
      return std::to_string(rank_) + "m";
    case SuitType::p:
      return std::to_string(rank_) + "p";
    case SuitType::s:
      return std::to_string(rank_) + "s";
  }
}

uint8_t SuitTile::to_id() noexcept {
  switch (type_) {
    case SuitType::m:
      return rank_;
    case SuitType::p:
      return 10 + rank_;
    case SuitType::s:
      return 20 + rank_;
  }
}

/**************************************
 *              HonorTile             *
 **************************************/
HonorTile::HonorTile(HonorType type) : type_(type) { Tile::category_ = TileCategory::Honor; }

std::string HonorTile::to_string() noexcept {
  switch (type_) {
    case HonorType::E:
      return "E";
    case HonorType::S:
      return "S";
    case HonorType::W:
      return "W";
    case HonorType::N:
      return "N";
    case HonorType::Z:
      return "Z";
    case HonorType::B:
      return "B";
    case HonorType::F:
      return "F";
  }
}

uint8_t HonorTile::to_id() noexcept { return static_cast<uint8_t>(type_) * 10; }

#endif