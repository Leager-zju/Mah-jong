#pragma once

#include <cstdint>
#include <unordered_map>

enum tile_id : uint16_t {
  _1m = 1,
  _2m,
  _3m,
  _4m,
  _5m,
  _6m,
  _7m,
  _8m,
  _9m,
  _1p = 11,
  _2p,
  _3p,
  _4p,
  _5p,
  _6p,
  _7p,
  _8p,
  _9p,
  _1s = 21,
  _2s,
  _3s,
  _4s,
  _5s,
  _6s,
  _7s,
  _8s,
  _9s,
  _E = 31,
  _S = 41,
  _W = 51,
  _N = 61,
  _Z = 71,
  _F = 81,
  _B = 91,
  NAT = UINT16_MAX  // not a tile
};
