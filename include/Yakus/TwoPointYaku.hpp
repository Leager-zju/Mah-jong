#pragma once

#include <cstdint>
#include <vector>

namespace MAHJONG {
enum class YakuType : uint8_t;
enum tile_id : uint16_t;
class MeldInId;
struct MatchResult;

// 纯/混全带幺九 副露减一番
class OutsideHand {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose, bool Menzenchin,
                        MatchResult& result);
};

// 一气通贯 副露减一番
class PureStraight {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose, bool Menzenchin,
                        MatchResult& result);
};

// 三色同顺 副露减一番
class MixedTripleSequence {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose, bool Menzenchin,
                        MatchResult& result);
};

// 三色同刻
class TripleTriplet {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 三杠子
class ThreeQuads {
 public:
  static void try_match(const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 对对和
class AllTriplet {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 三暗刻
class ThreeConcealedTriplets {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 混老头
class AllTerminalsAndHonors {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 七对子 门前清限定
class SevenPairs {
 public:
  static void try_match(const class Hand& hand, tile_id new_tile_id,
                        MatchResult& result);
};
};  // namespace MAHJONG