#pragma once

#include <cstdint>
#include <vector>

namespace MAHJONG {
enum class YakuType : uint8_t;
enum tile_id : uint16_t;
class MeldInId;
struct MatchResult;

// 混/清一色 副露减一番
class Flush {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose, bool Menzenchin,
                        MatchResult& result);
};

// 二杯口 门前清限定
class TwicePureDoubleSequence {
 public:
  static void try_match(const std::vector<MeldInId>& hand, MatchResult& result);
};
};  // namespace MAHJONG