#include "ThreePointYaku.hpp"

#include "Common.hpp"
#include "Meld.hpp"
#include "YakusMatcher.hpp"

#include <cstdint>
#include <unordered_map>

namespace mahjong {
void Flush::TryMatch(const std::vector<MeldInId>& hand,
                     const std::vector<MeldInId>& expose,
                     bool Menzenchin,
                     MatchResult& result) {
  uint8_t category = UINT8_MAX;
  bool half        = false;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsHonor(tile_ids[0])) {
      half = true;
    } else if (category == UINT8_MAX) {
      category = tile_ids[0] / 10;
    } else if (tile_ids[0] / 10 != category) {
      return;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsHonor(tile_ids[0])) {
      half = true;
    } else if (category == UINT8_MAX) {
      category = tile_ids[0] / 10;
    } else if (tile_ids[0] / 10 != category) {
      return;
    }
  }
  if (half) {
    if (Menzenchin) {
      result.AddPoint(3);
      result.AddYaku(YakuType::HalfFlushWithMenzenchin);
    } else {
      result.AddPoint(2);
      result.AddYaku(YakuType::HalfFlush);
    }
  } else {
    if (Menzenchin) {
      result.AddPoint(3);
      result.AddYaku(YakuType::FullyFlushWithMenzenchin);
    } else {
      result.AddPoint(2);
      result.AddYaku(YakuType::FullyFlush);
    }
  }
}

void TwicePureDoubleSequence::TryMatch(const std::vector<MeldInId>& hand,
                                       MatchResult& result) {
  std::unordered_map<TileId, uint8_t> counts;
  for (auto&& meld : hand) {
    if (meld.GetMeldType() == MeldType::Sequence) {
      auto&& tile_ids = meld.GetTileId();
      if (counts.count(tile_ids[0])) {
        counts.insert({tile_ids[0], 1});
      } else {
        counts[tile_ids[0]]++;
      }
    }
  }

  if (counts.size() != 2) {
    return;
  }

  for (auto&& count : counts) {
    if (count.second != 2) {
      return;
    }
  }
  result.AddPoint(2);
  result.AddYaku(YakuType::TwicePureDoubleSequence);
}
};  // namespace mahjong