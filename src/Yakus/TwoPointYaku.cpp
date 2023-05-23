#include "TwoPointYaku.hpp"

#include "Common.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"
#include "YakusMatcher.hpp"

#include <cstdint>
#include <unordered_map>

namespace mahjong {
void OutsideHand::TryMatch(const std::vector<MeldInId>& hand,
                           const std::vector<MeldInId>& expose,
                           bool Menzenchin,
                           MatchResult& result) {
  bool half = false;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (!Tile::IsTerminal(tile_ids[0])
        && (meld.GetMeldType() != MeldType::Sequence
            || !Tile::IsTerminal(tile_ids[2]))) {
      return;
    }

    if (!half && Tile::IsHonor(tile_ids[0])) {
      half = true;
    }
  }

  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (!Tile::IsTerminal(tile_ids[0])
        && (meld.GetMeldType() != MeldType::Sequence
            || !Tile::IsTerminal(tile_ids[2]))) {
      return;
    }
    if (!half && Tile::IsHonor(tile_ids[0])) {
      half = true;
    }
  }

  if (half) {
    if (Menzenchin) {
      result.AddPoint(1);
      result.AddYaku(YakuType::HalfOutsideHandWithMenzenchin);
    } else {
      result.AddPoint(2);
      result.AddYaku(YakuType::HalfOutsideHand);
    }
  } else {
    if (Menzenchin) {
      result.AddPoint(3);
      result.AddYaku(YakuType::FullyOutsideHandWithMenzenchin);
    } else {
      result.AddPoint(2);
      result.AddYaku(YakuType::FullyOutsideHand);
    }
  }
}

void PureStraight::TryMatch(const std::vector<MeldInId>& hand,
                            const std::vector<MeldInId>& expose,
                            bool Menzenchin,
                            MatchResult& result) {
  std::vector<TileId> temp;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence) {
      temp.push_back(tile_ids[0]);
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence) {
      temp.push_back(tile_ids[0]);
    }
  }

  if (temp.size() < 3) {
    return;
  }

  std::sort(temp.begin(), temp.end());
  for (size_t i = 0; i < temp.size() - 2; i++) {
    if (temp[i] % 10 == 1) {
      if ((temp[i + 1] % 10 == 4 && temp[i + 1] / 10 == temp[i] / 10)
          && (temp[i + 2] % 10 == 7 && temp[i + 2] / 10 == temp[i] / 10)) {
        if (Menzenchin) {
          result.AddPoint(2);
          result.AddYaku(YakuType::PureStraightWithMenzenchin);
        } else {
          result.AddPoint(1);
          result.AddYaku(YakuType::PureStraight);
        }
      }
    }
  }
}

void MixedTripleSequence::TryMatch(const std::vector<MeldInId>& hand,
                                   const std::vector<MeldInId>& expose,
                                   bool Menzenchin,
                                   MatchResult& result) {
  std::vector<TileId> temp;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence) {
      temp.push_back(tile_ids[0]);
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence) {
      temp.push_back(tile_ids[0]);
    }
  }

  if (temp.size() < 3) {
    return;
  }

  std::sort(temp.begin(), temp.end());
  for (size_t i = 0; i < temp.size() - 2; i++) {
    uint8_t cate1 = temp[i] / 10;
    uint8_t cate2 = temp[i + 1] / 10;
    uint8_t cate3 = temp[i + 2] / 10;
    uint8_t rank1 = temp[i] % 10;
    uint8_t rank2 = temp[i + 1] % 10;
    uint8_t rank3 = temp[i + 2] % 10;
    if (cate1 != cate2 && cate1 != cate3 && cate2 != cate3 && rank1 == rank2
        && rank1 == rank3) {
      if (Menzenchin) {
        result.AddPoint(2);
        result.AddYaku(YakuType::MixedTripleSequenceWithMenzenchin);
      } else {
        result.AddPoint(1);
        result.AddYaku(YakuType::MixedTripleSequence);
      }
    }
  }
}

void TripleTriplet::TryMatch(const std::vector<MeldInId>& hand,
                             const std::vector<MeldInId>& expose,
                             MatchResult& result) {
  std::unordered_map<TileId, uint8_t> triplet_count;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Triplet
        || meld.GetMeldType() == MeldType::ConcealedKong
        || meld.GetMeldType() == MeldType::ExposeKong) {
      if (!triplet_count.count(tile_ids[0])) {
        triplet_count.insert({tile_ids[0], 1});
      } else if (triplet_count[tile_ids[0]] == 2) {
        result.AddPoint(2);
        result.AddYaku(YakuType::TripleTriplet);
        return;
      } else {
        triplet_count[tile_ids[0]]++;
      }
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Triplet
        || meld.GetMeldType() == MeldType::ConcealedKong
        || meld.GetMeldType() == MeldType::ExposeKong) {
      if (!triplet_count.count(tile_ids[0])) {
        triplet_count.insert({tile_ids[0], 1});
      } else if (triplet_count[tile_ids[0]] == 2) {
        result.AddPoint(2);
        result.AddYaku(YakuType::TripleTriplet);
        return;
      } else {
        triplet_count[tile_ids[0]]++;
      }
    }
  }
}

void ThreeQuads::TryMatch(const std::vector<MeldInId>& expose,
                          MatchResult& result) {
  uint16_t count = 0;
  for (auto&& meld : expose) {
    if (meld.GetMeldType() == MeldType::ConcealedKong
        || meld.GetMeldType() == MeldType::ExposeKong) {
      count++;
    }
  }
  if (count == 3) {
    result.AddPoint(2);
    result.AddYaku(YakuType::ThreeQuads);
  }
}

void AllTriplet::TryMatch(const std::vector<MeldInId>& hand,
                          const std::vector<MeldInId>& expose,
                          MatchResult& result) {
  for (auto&& meld : hand) {
    if (meld.GetMeldType() == MeldType::Sequence) {
      return;
    }
  }
  for (auto&& meld : expose) {
    if (meld.GetMeldType() == MeldType::Sequence) {
      return;
    }
  }
  result.AddPoint(2);
  result.AddYaku(YakuType::AllTriplet);
}

void ThreeConcealedTriplets::TryMatch(const std::vector<MeldInId>& hand,
                                      const std::vector<MeldInId>& expose,
                                      MatchResult& result) {
  uint8_t count = 0;
  for (auto&& meld : hand) {
    if (meld.GetMeldType() == MeldType::Triplet) {
      count++;
    }
  }
  for (auto&& meld : expose) {
    if (meld.GetMeldType() == MeldType::ConcealedKong) {
      count++;
    }
  }
  if (count == 3) {
    result.AddPoint(2);
    result.AddYaku(YakuType::ThreeConcealedTriplets);
  }
}

void AllTerminalsAndHonors::TryMatch(const std::vector<MeldInId>& hand,
                                     const std::vector<MeldInId>& expose,
                                     MatchResult& result) {
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence
        || !Tile::IsTerminal(tile_ids[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence
        || !Tile::IsTerminal(tile_ids[0])) {
      return;
    }
  }
  result.AddPoint(2);
  result.AddYaku(YakuType::AllTerminalsAndHonors);
}

void SevenPairs::TryMatch(const Hand& hand,
                          TileId new_tile_id,
                          MatchResult& result) {
  std::unordered_map<TileId, uint8_t> counts;
  for (auto&& tile : hand.GetHands()) {
    TileId id = tile->GetId();
    if (!counts.count(id)) {
      counts.insert({id, 1});
    } else {
      counts[id]++;
    }
  }
  if (!counts.count(new_tile_id)) {
    counts.insert({new_tile_id, 1});
  } else {
    counts[new_tile_id]++;
  }

  if (counts.size() != 7) {
    return;
  }
  for (auto&& count : counts) {
    if (count.second != 2) {
      return;
    }
  }

  result.AddPoint(2);
  result.AddYaku(YakuType::SevenPairs);
}
};  // namespace mahjong