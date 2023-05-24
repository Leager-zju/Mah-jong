#pragma once

#include "TwoPointYaku.hpp"

#include <cstdint>

namespace mahjong {
const uint16_t TOTAL_TILES = 136;

const uint16_t TILE_LOWER_BOUND             = 0;
const uint16_t TILE_UPPER_BOUND             = 121;
const uint16_t DORA_TILE_LOWER_BOUND        = 122;
const uint16_t DORA_TILE_UPPER_BOUND        = 131;
const uint16_t REPLACEMENT_TILE_LOWER_BOUND = 132;
const uint16_t REPLACEMENT_TILE_UPPER_BOUND = 135;

const uint16_t MAN = 0;
const uint16_t PIN = 10;
const uint16_t SOU = 20;

const uint16_t TOTAL_PLAYERS = 4;

enum class YakuType : uint8_t {
  // 一番
  Tsumo,
  Riichi,
  UnderTheSea,
  UnderTheRiver,
  SeatWind,
  PrevalentWind,
  RedDragon,
  WhiteDragon,
  GreenDragon,
  AllSimple,
  PureDoubleSequence,
  Pinfu,

  // 二番
  HalfOutsideHandWithMenzenchin,
  HalfOutsideHand,
  PureStraightWithMenzenchin,
  PureStraight,
  MixedTripleSequenceWithMenzenchin,
  MixedTripleSequence,
  TripleTriplet,
  ThreeQuads,
  AllTriplet,
  ThreeConcealedTriplets,
  AllTerminalsAndHonors,
  SevenPairs,
  SmallThreeDragons,

  // 三番
  FullyOutsideHandWithMenzenchin,
  FullyOutsideHand,
  HalfFlushWithMenzenchin,
  HalfFlush,
  TwicePureDoubleSequence,

  // 六番
  FullyFlushWithMenzenchin,
  FullyFlush,

  // 役满
  BlessingOfHeaven,
  BlessingOfEarth,
  BigThreeDragons,
  FourConcealedTriplets,
  AllHonors,
  AllGreen,
  AllTerminals,
  ThirteenOrphans,
  SmallFourWinds,
  FourQuads,
  NineGates,

  // 两倍役满
  PureNineGates,
  SingleWaitFourConcealedTriplets,
  ThirteenWaitThirteenOrphans,
  BigFourWinds
};

enum TileId : uint16_t {
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
  E = 31,
  S = 41,
  W = 51,
  N = 61,
  Z = 71,
  B = 81,
  F = 91,

  NAT = UINT16_MAX  // not a tile
};
};  // namespace mahjong
