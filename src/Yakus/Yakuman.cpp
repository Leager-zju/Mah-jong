#include "Yakuman.hpp"

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#include "Common.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"
#include "YakuMatcher.hpp"

namespace MAHJONG {
void BigThreeDragons::try_match(const std::vector<MeldInId>& hand,
                                const std::vector<MeldInId>& expose,
                                MatchResult& result) {
  uint8_t count = 0;
  for (auto&& meld : hand) {
    if (Tile::is_dragon(meld.tile_ids_[0])) {
      count++;
    }
  }
  for (auto&& meld : expose) {
    if (Tile::is_dragon(meld.tile_ids_[0])) {
      count++;
    }
  }

  if (count == 3) {
    result.yakuman_ += 1;
    result.Yakus_matched_.push_back(YakuType::BigThreeDragons);
  }
}

void FourConcealedTriplets::try_match(const std::vector<MeldInId>& hand,
                                      const std::vector<MeldInId>& expose,
                                      tile_id new_tile_id, bool self_drawn,
                                      MatchResult& result) {
  uint8_t count = 0;
  bool SingleWait = false;
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Eyes && meld.tile_ids_[0] == new_tile_id) {
      SingleWait = true;
    }
    if (meld.meld_type_ == MeldType::Triplet) {
      count++;
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::ConcealedKong) {
      count++;
    }
  }
  if (count != 4) return;

  if (SingleWait) {
    result.yakuman_ += 2;
    result.Yakus_matched_.push_back(YakuType::SingleWaitFourConcealedTriplets);
  } else if (self_drawn) {
    result.yakuman_ += 1;
    result.Yakus_matched_.push_back(YakuType::FourConcealedTriplets);
  }
}

void AllHonors::try_match(const std::vector<MeldInId>& hand,
                          const std::vector<MeldInId>& expose,
                          MatchResult& result) {
  for (auto&& meld : hand) {
    if (!Tile::is_honor(meld.tile_ids_[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    if (!Tile::is_honor(meld.tile_ids_[0])) {
      return;
    }
  }

  result.yakuman_ += 1;
  result.Yakus_matched_.push_back(YakuType::AllHonors);
}

void AllGreen::try_match(const std::vector<MeldInId>& hand,
                         const std::vector<MeldInId>& expose,
                         MatchResult& result) {
  auto is_green = [](tile_id id) -> bool {
    return id == _2s || id == _3s || id == _4s || id == _6s || id == _8s ||
           id == _F;
  };

  for (auto&& meld : hand) {
    if (!is_green(meld.tile_ids_[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    if (!is_green(meld.tile_ids_[0])) {
      return;
    }
  }

  result.yakuman_ += 1;
  result.Yakus_matched_.push_back(YakuType::AllGreen);
}

void AllTerminals::try_match(const std::vector<MeldInId>& hand,
                             const std::vector<MeldInId>& expose,
                             MatchResult& result) {
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence ||
        Tile::is_honor(meld.tile_ids_[0]) ||
        !Tile::is_terminal(meld.tile_ids_[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::Sequence ||
        Tile::is_honor(meld.tile_ids_[0]) ||
        !Tile::is_terminal(meld.tile_ids_[0])) {
      return;
    }
  }

  result.yakuman_ += 1;
  result.Yakus_matched_.push_back(YakuType::AllTerminals);
}

void ThirteenOrphans::try_match(const Hand& hand, tile_id new_tile_id,
                                MatchResult& result) {
  std::unordered_set<tile_id> s;
  for (auto&& tile : hand.get_hands()) {
    s.insert(tile->to_id());
  }
  if (s.size() == 12 && s.find(new_tile_id) == s.end()) {
    result.yakuman_ += 1;
    result.Yakus_matched_.push_back(YakuType::ThirteenOrphans);
  } else if (s.size() == 13) {
    result.yakuman_ += 2;
    result.Yakus_matched_.push_back(YakuType::ThirteenWaitThirteenOrphans);
  }
}

void FourWind::try_match(const std::vector<MeldInId>& hand,
                         const std::vector<MeldInId>& expose,
                         MatchResult& result) {
  uint8_t count = 0;
  bool wind_eye = false;
  for (auto&& meld : hand) {
    if (Tile::is_wind(meld.tile_ids_[0])) {
      if (meld.meld_type_ == MeldType::Eyes) {
        wind_eye = true;
      }
      count++;
    }
  }
  for (auto&& meld : expose) {
    if (Tile::is_wind(meld.tile_ids_[0])) {
      count++;
    }
  }
  if (count == 3 && wind_eye) {
    result.yakuman_ += 1;
    result.Yakus_matched_.push_back(YakuType::SmallFourWinds);
  } else if (count == 4) {
    result.yakuman_ += 2;
    result.Yakus_matched_.push_back(YakuType::BigFourWinds);
  }
}

void FourQuads::try_match(const std::vector<MeldInId>& hand,
                          const std::vector<MeldInId>& expose,
                          MatchResult& result) {
  uint8_t count = 0;
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::ConcealedKong ||
        meld.meld_type_ == MeldType::ExposeKong) {
      count++;
    }
  }
  if (count == 4) {
    result.yakuman_ += 1;
    result.Yakus_matched_.push_back(YakuType::FourQuads);
  }
}

void NineGates::try_match(const Hand& hand, tile_id new_tile_id,
                          MatchResult& result) {
  std::unordered_map<tile_id, uint8_t> counts;
  uint8_t category = hand.get_hands().front()->to_id() / 10;

  for (auto&& tile : hand.get_hands()) {
    tile_id id = tile->to_id();
    if (id / 10 != category) {
      return;
    }
    if (counts.count(id)) {
      counts[id]++;
    } else {
      counts.insert({id, 1});
    }
  }
  if (counts.count(new_tile_id)) {
    counts[new_tile_id]++;
  } else {
    counts.insert({new_tile_id, 1});
  }

  // 311..2..113 or 411..113 or 311..114
  tile_id one = static_cast<tile_id>(category * 10 + 1);
  tile_id nine = static_cast<tile_id>(category * 10 + 9);

  if (counts.size() < 9 || counts[one] < 3 || counts[nine] < 3) return;

  bool pure = ((new_tile_id == one || new_tile_id == nine) &&
               counts[new_tile_id] > 3) ||
              (counts[new_tile_id] > 1);

  if (pure) {
    result.yakuman_ += 2;
    result.Yakus_matched_.push_back(YakuType::PureNineGates);
  } else {
    result.yakuman_ += 1;
    result.Yakus_matched_.push_back(YakuType::NineGates);
  }
}
};  // namespace MAHJONG