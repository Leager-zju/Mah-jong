#pragma once

#include <cstdint>
#include <vector>

namespace mahjong {
enum class YakuType : uint8_t;
enum TileId : uint16_t;
class MeldInId;
class Hand;
struct WinningResult;

// 大三元
class BigThreeDragons {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 四暗刻 [自摸限定]/四暗刻单骑 门前清限定
class FourConcealedTriplets {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       TileId new_tile_id,
                       bool self_drawn,
                       WinningResult& result);
};

// 字一色
class AllHonors {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 绿一色
class AllGreen {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 清老头
class AllTerminals {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 国士无双/国士无双十三面 门前清限定
class ThirteenOrphans {
 public:
  static void TryMatch(const Hand& hand,
                       TileId new_tile_id,
                       WinningResult& result);
};

// 小四喜/大四喜
class FourWind {
 public:
  static void TryMatch(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 四杠子
class FourQuads {
 public:
  static void TryMatch(const std::vector<MeldInId>& expose,
                       WinningResult& result);
};

// 九莲宝灯/纯正九莲宝灯 门前清限定
class NineGates {
 public:
  static void TryMatch(const Hand& hand,
                       TileId new_tile_id,
                       WinningResult& result);
};
};  // namespace mahjong