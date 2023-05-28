#pragma once

#include <cstdint>
#include <vector>

namespace mahjong {
enum class YakuType : uint8_t;
enum TileId : uint16_t;
class MeldInId;
struct WinningResult;

// 混/清一色 副露减一番
class Flush {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       bool Menzenchin,
                       WinningResult& result);
};

// 二杯口 门前清限定
class TwicePureDoubleSequence {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       WinningResult& result);
};
};  // namespace mahjong