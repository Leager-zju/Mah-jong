#include "ThreePointYaku.hpp"

#include <cstdint>
#include <unordered_map>

#include "Common.hpp"
#include "Meld.hpp"
#include "YakuMatcher.hpp"

namespace MAHJONG {
void Flush::try_match(const std::vector<MeldInId>& hand,
                      const std::vector<MeldInId>& expose, bool Menzenchin,
                      MatchResult& result) {
  uint8_t category = UINT8_MAX;
  bool half = false;
  for (auto&& meld : hand) {
    if (Tile::is_honor(meld.tile_ids_[0])) {
      half = true;
    } else if (category == UINT8_MAX) {
      category = meld.tile_ids_[0] / 10;
    } else if (meld.tile_ids_[0] / 10 != category) {
      return;
    }
  }
  if (half) {
    if (Menzenchin) {
      result.points_ += 3;
      result.Yakus_matched_.push_back(YakuType::HalfFlushWithMenzenchin);
    } else {
      result.points_ += 2;
      result.Yakus_matched_.push_back(YakuType::HalfFlush);
    }
  } else {
    if (Menzenchin) {
      result.points_ += 3;
      result.Yakus_matched_.push_back(YakuType::FullyFlushWithMenzenchin);
    } else {
      result.points_ += 2;
      result.Yakus_matched_.push_back(YakuType::FullyFlush);
    }
  }
}

void TwicePureDoubleSequence::try_match(const std::vector<MeldInId>& hand,
                                        MatchResult& result) {
  std::unordered_map<tile_id, uint8_t> counts;
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence) {
      if (counts.count(meld.tile_ids_[0])) {
        counts.insert({meld.tile_ids_[0], 1});
      } else {
        counts[meld.tile_ids_[0]]++;
      }
    }
  }

  if (counts.size() != 2) return;

  for (auto&& count : counts) {
    if (count.second != 2) {
      return;
    }
  }
  result.points_ += 2;
  result.Yakus_matched_.push_back(YakuType::TwicePureDoubleSequence);
}
};  // namespace MAHJONG