#pragma once

#include <cstdint>

#include "TwoPointYaku.hpp"

namespace MAHJONG {
const uint16_t TOTAL = 136;

const uint16_t TILE_LOWER_BOUND = 0;
const uint16_t TILE_UPPER_BOUND = 121;
const uint16_t DORA_TILE_LOWER_BOUND = 122;
const uint16_t DORA_TILE_UPPER_BOUND = 131;
const uint16_t REPLACEMENT_TILE_LOWER_BOUND = 132;
const uint16_t REPLACEMENT_TILE_UPPER_BOUND = 135;

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
  _B = 81,
  _F = 91,

  NAT = UINT16_MAX  // not a tile
};
};  // namespace MAHJONG
