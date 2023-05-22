#include "TwoPointYaku.hpp"

#include <cstdint>
#include <unordered_map>

#include "Common.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"
#include "YakuMatcher.hpp"

namespace MAHJONG {
void OutsideHand::try_match(const std::vector<MeldInId>& hand,
                            const std::vector<MeldInId>& expose,
                            bool Menzenchin, MatchResult& result) {
  bool half = false;
  for (auto&& meld : hand) {
    if (!Tile::is_terminal(meld.tile_ids_[0]) &&
        !(meld.meld_type_ == MeldType::Sequence &&
          Tile::is_terminal(meld.tile_ids_[2]))) {
      return;
    }

    if (!half && Tile::is_honor(meld.tile_ids_[0])) {
      half = true;
    }
  }

  for (auto&& meld : expose) {
    if (!Tile::is_terminal(meld.tile_ids_[0]) &&
        !(meld.meld_type_ == MeldType::Sequence &&
          Tile::is_terminal(meld.tile_ids_[2]))) {
      return;
    }
  }

  if (half) {
    if (Menzenchin) {
      result.points_ += 1;
      result.Yakus_matched_.push_back(YakuType::HalfOutsideHandWithMenzenchin);
    } else {
      result.points_ += 2;
      result.Yakus_matched_.push_back(YakuType::HalfOutsideHand);
    }
  } else {
    if (Menzenchin) {
      result.points_ += 3;
      result.Yakus_matched_.push_back(YakuType::FullyOutsideHandWithMenzenchin);
    } else {
      result.points_ += 2;
      result.Yakus_matched_.push_back(YakuType::FullyOutsideHand);
    }
  }
}

void PureStraight::try_match(const std::vector<MeldInId>& hand,
                             const std::vector<MeldInId>& expose,
                             bool Menzenchin, MatchResult& result) {
  std::vector<tile_id> temp;
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence) {
      temp.push_back(meld.tile_ids_[0]);
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::Sequence) {
      temp.push_back(meld.tile_ids_[0]);
    }
  }

  if (temp.size() < 3) return;

  std::sort(temp.begin(), temp.end());
  for (size_t i = 0; i < temp.size() - 2; i++) {
    if (temp[i] % 10 == 1) {
      if ((temp[i + 1] % 10 == 4 && temp[i + 1] / 10 == temp[i] / 10) &&
          (temp[i + 2] % 10 == 7 && temp[i + 2] / 10 == temp[i] / 10)) {
        if (Menzenchin) {
          result.points_ += 2;
          result.Yakus_matched_.push_back(YakuType::PureStraightWithMenzenchin);
        } else {
          result.points_ += 1;
          result.Yakus_matched_.push_back(YakuType::PureStraight);
        }
      }
    }
  }
}

void MixedTripleSequence::try_match(const std::vector<MeldInId>& hand,
                                    const std::vector<MeldInId>& expose,
                                    bool Menzenchin, MatchResult& result) {
  std::vector<tile_id> temp;
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence) {
      temp.push_back(meld.tile_ids_[0]);
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::Sequence) {
      temp.push_back(meld.tile_ids_[0]);
    }
  }

  if (temp.size() < 3) return;

  std::sort(temp.begin(), temp.end());
  for (size_t i = 0; i < temp.size() - 2; i++) {
    uint8_t cate1 = temp[i] / 10;
    uint8_t cate2 = temp[i + 1] / 10;
    uint8_t cate3 = temp[i + 2] / 10;
    uint8_t rank1 = temp[i] % 10;
    uint8_t rank2 = temp[i + 1] % 10;
    uint8_t rank3 = temp[i + 2] % 10;
    if (cate1 != cate2 && cate1 != cate3 && cate2 != cate3 && rank1 == rank2 &&
        rank1 == rank3) {
      if (Menzenchin) {
        result.points_ += 2;
        result.Yakus_matched_.push_back(
            YakuType::MixedTripleSequenceWithMenzenchin);
      } else {
        result.points_ += 1;
        result.Yakus_matched_.push_back(YakuType::MixedTripleSequence);
      }
    }
  }
}

void TripleTriplet::try_match(const std::vector<MeldInId>& hand,
                              const std::vector<MeldInId>& expose,
                              MatchResult& result) {
  std::unordered_map<tile_id, uint8_t> triplet_count;
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Triplet ||
        meld.meld_type_ == MeldType::ConcealedKong ||
        meld.meld_type_ == MeldType::ExposeKong) {
      if (triplet_count.count(meld.tile_ids_[0])) {
        triplet_count.insert({meld.tile_ids_[0], 1});
      } else if (triplet_count[meld.tile_ids_[0]] == 2) {
        result.points_ += 2;
        result.Yakus_matched_.push_back(YakuType::TripleTriplet);
        return;
      } else {
        triplet_count[meld.tile_ids_[0]]++;
      }
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::Triplet ||
        meld.meld_type_ == MeldType::ConcealedKong ||
        meld.meld_type_ == MeldType::ExposeKong) {
      if (triplet_count.count(meld.tile_ids_[0])) {
        triplet_count.insert({meld.tile_ids_[0], 1});
      } else if (triplet_count[meld.tile_ids_[0]] == 2) {
        result.points_ += 2;
        result.Yakus_matched_.push_back(YakuType::TripleTriplet);
        return;
      } else {
        triplet_count[meld.tile_ids_[0]]++;
      }
    }
  }
}

void ThreeQuads::try_match(const std::vector<MeldInId>& expose,
                           MatchResult& result) {
  uint16_t count = 0;
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::ConcealedKong ||
        meld.meld_type_ == MeldType::ExposeKong) {
      count++;
    }
  }
  if (count == 3) {
    result.points_ += 2;
    result.Yakus_matched_.push_back(YakuType::ThreeQuads);
  }
}

void AllTriplet::try_match(const std::vector<MeldInId>& hand,
                           const std::vector<MeldInId>& expose,
                           MatchResult& result) {
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence) {
      return;
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::Sequence) {
      return;
    }
  }
  result.points_ += 2;
  result.Yakus_matched_.push_back(YakuType::AllTriplet);
}

void ThreeConcealedTriplets::try_match(const std::vector<MeldInId>& hand,
                                       const std::vector<MeldInId>& expose,
                                       MatchResult& result) {
  uint8_t count = 0;
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Triplet) {
      count++;
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::ConcealedKong) {
      count++;
    }
  }
  if (count == 3) {
    result.points_ += 2;
    result.Yakus_matched_.push_back(YakuType::ThreeConcealedTriplets);
  }
}

void AllTerminalsAndHonors::try_match(const std::vector<MeldInId>& hand,
                                      const std::vector<MeldInId>& expose,
                                      MatchResult& result) {
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence ||
        !Tile::is_terminal(meld.tile_ids_[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::Sequence ||
        !Tile::is_terminal(meld.tile_ids_[0])) {
      return;
    }
  }
  result.points_ += 2;
  result.Yakus_matched_.push_back(YakuType::AllTerminalsAndHonors);
}

void SevenPairs::try_match(const Hand& hand, tile_id new_tile_id,
                           MatchResult& result) {
  std::unordered_map<tile_id, uint8_t> counts;
  for (auto&& tile : hand.get_hands()) {
    tile_id id = tile->to_id();
    if (counts.count(id)) {
      counts.insert({id, 1});
    } else {
      counts[id]++;
    }
  }
  if (counts.count(new_tile_id)) {
    counts.insert({new_tile_id, 1});
  } else {
    counts[new_tile_id]++;
  }

  if (counts.size() != 7) return;
  for (auto&& count : counts) {
    if (count.second != 2) {
      return;
    }
  }

  result.points_ += 2;
  result.Yakus_matched_.push_back(YakuType::SevenPairs);
}
};  // namespace MAHJONG