#pragma once

#include <cstdint>

enum class TileCategory : uint16_t {
  Suit = 0,  // 万筒条
  Honor,     // 东南西北中白发
};

enum class SuitType {
  m = 0,  // 万
  p,      // 筒
  s,      // 条
};

enum class HonorType : uint16_t {
  E = 3,  // 东
  S,      // 南
  W,      // 西
  N,      // 北
  Z,      // 中
  B,      // 白
  F,      // 发
};