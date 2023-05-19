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
pTile Tile::from_string(const std::string& str) {
  assert(!str.empty() && str.length() <= 2);
  if (str.length() == 1) {
    switch (str[0]) {
      case 'E':
        return std::make_unique<HonorTile>(HonorType::E);
      case 'S':
        return std::make_unique<HonorTile>(HonorType::S);
      case 'W':
        return std::make_unique<HonorTile>(HonorType::W);
      case 'N':
        return std::make_unique<HonorTile>(HonorType::N);
      case 'Z':
        return std::make_unique<HonorTile>(HonorType::Z);
      case 'B':
        return std::make_unique<HonorTile>(HonorType::B);
      case 'F':
        return std::make_unique<HonorTile>(HonorType::F);
      default:
        return nullptr;
    }
  }

  uint16_t rank = str[0] - '0';
  switch (str[1]) {
    case 'm':
      return std::make_unique<SuitTile>(SuitType::m, rank);
    case 'p':
      return std::make_unique<SuitTile>(SuitType::p, rank);
    case 's':
      return std::make_unique<SuitTile>(SuitType::s, rank);
    default:
      return nullptr;
  }
}

pTile Tile::from_id(tile_id_t id) {
  uint16_t type = id / 10;
  uint16_t rank = id % 10;
  if (type <= 2) {
    return std::make_unique<SuitTile>(static_cast<SuitType>(type), rank);
  }
  assert(type >= 3 && type <= 9 && rank == 0);
  return std::make_unique<HonorTile>(static_cast<HonorType>(type));
}

tile_id_t Tile::transform_string2id(const std::string& str) {
  return from_string(str)->to_id();
}

std::string Tile::transform_id2string(tile_id_t id) {
  return from_id(id)->to_string();
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

/**************************************
 *              SuitTile              *
 **************************************/
SuitTile::SuitTile(SuitType type, uint16_t rank) : type_(type), rank_(rank) { Tile::category_ = TileCategory::Suit; }

std::string SuitTile::to_string() noexcept {
  std::string res = get_ANSI();

  res.push_back('0' + rank_);
  switch (type_) {
    case SuitType::m:
      res.push_back('m');
      break;
    case SuitType::p:
      res.push_back('p');
      break;
    case SuitType::s:
      res.push_back('s');
      break;
  }
  res += "\033[0m";

  return res;
}

tile_id_t SuitTile::to_id() noexcept {
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
  std::string res = get_ANSI();

  switch (type_) {
    case HonorType::E:
      res.push_back('E');
      break;
    case HonorType::S:
      res.push_back('S');
      break;
    case HonorType::W:
      res.push_back('W');
      break;
    case HonorType::N:
      res.push_back('N');
      break;
    case HonorType::Z:
      res.push_back('Z');
      break;
    case HonorType::B:
      res.push_back('B');
      break;
    case HonorType::F:
      res.push_back('F');
      break;
  }

  res += "\033[0m";

  return res;
}

tile_id_t HonorTile::to_id() noexcept { return static_cast<tile_id_t>(type_) * 10; }

#endif