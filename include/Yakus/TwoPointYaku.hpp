#pragma once

#include <cstdint>
#include <vector>

namespace mahjong {
enum class YakuType : uint8_t;
enum TileId : uint16_t;
class MeldInId;
struct WinningResult;

// 纯/混全带幺九 副露减一番
class OutsideHand {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       bool Menzenchin,
                       WinningResult& result);
};

// 一气通贯 副露减一番
class PureStraight {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       bool Menzenchin,
                       WinningResult& result);
};

// 三色同顺 副露减一番
class MixedTripleSequence {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       bool Menzenchin,
                       WinningResult& result);
};

// 三色同刻
class TripleTriplet {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 三杠子
class ThreeQuads {
 public:
  static void TryMatch(const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 对对和
class AllTriplet {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 三暗刻
class ThreeConcealedTriplets {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 混老头
class AllTerminalsAndHonors {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 七对子 门前清限定
class SevenPairs {
 public:
  static void TryMatch(const class Hand& hand,
                       TileId new_tile_id,
                       WinningResult& result);
};
};  // namespace mahjong