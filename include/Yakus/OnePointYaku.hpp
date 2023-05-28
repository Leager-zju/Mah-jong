#pragma once

#include <cstdint>
#include <vector>

namespace mahjong {
enum class YakuType : uint8_t;
enum TileId : uint16_t;
class MeldInId;
struct WinningResult;

// 门前清自摸和 立直限定
class Tsumo {
 public:
  static void TryMatch(bool self_drawn, WinningResult& result);
};

// 立直
class Riichi {
 public:
  static void TryMatch(WinningResult& result);
};

// 枪杠
class RobbingKan {  // TODO(Leager)
};

// 岭上开花
class AfterKan {  // TODO(Leager)
};

// 海底捞月/河底捞鱼
class UnderTheSeaOrRiver {
 public:
  static void TryMatch(bool self_drawn, WinningResult& result);
};

// 役牌 风
class Wind {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 役牌 三元
class Dragon {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 断幺九
class AllSimple {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 一杯口 门前清限定
class PureDoubleSequence {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       WinningResult& result);
};

// 平和 门前清限定
class Pinfu {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       TileId new_tile_id,
                       WinningResult& result);
};

// 一发 立直限定
class Ippatsu {
 public:
  static void TryMatch(WinningResult& result);
};
};  // namespace mahjong