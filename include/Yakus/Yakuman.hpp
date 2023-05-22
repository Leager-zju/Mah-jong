#pragma once

#include <cstdint>
#include <vector>

namespace MAHJONG {
enum class YakuType : uint8_t;
enum tile_id : uint16_t;
class MeldInId;
class Hand;
struct MatchResult;

// 大三元
class BigThreeDragons {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 四暗刻 [自摸限定]/四暗刻单骑 门前清限定
class FourConcealedTriplets {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        tile_id new_tile_id, bool self_drawn,
                        MatchResult& result);
};

// 字一色
class AllHonors {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 绿一色
class AllGreen {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 清老头
class AllTerminals {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 国士无双/国士无双十三面 门前清限定
class ThirteenOrphans {
 public:
  static void try_match(const Hand& hand, tile_id new_tile_id,
                        MatchResult& result);
};

// 小四喜/大四喜
class FourWind {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 四杠子
class FourQuads {
 public:
  static void try_match(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        MatchResult& result);
};

// 九莲宝灯/纯正九莲宝灯 门前清限定
class NineGates {
 public:
  static void try_match(const Hand& hand, tile_id new_tile_id,
                        MatchResult& result);
};
};  // namespace MAHJONG