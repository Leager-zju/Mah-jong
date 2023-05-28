#include "Yakuman.hpp"

#include "Common.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"
#include "YakusMatcher.hpp"

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

namespace mahjong {
void BigThreeDragons::TryMatch(const std::vector<MeldInId>& hand,
                               const std::vector<MeldInId>& expose,
                               WinningResult& result) {
  uint8_t count = 0;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsDragon(tile_ids[0])) {
      count++;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsDragon(tile_ids[0])) {
      count++;
    }
  }

  if (count == 3) {
    result.AddYakuman(1);
    result.AddYaku(YakuType::BigThreeDragons);
  }
}

void FourConcealedTriplets::TryMatch(const std::vector<MeldInId>& hand,
                                     const std::vector<MeldInId>& expose,
                                     TileId new_tile_id,
                                     bool self_drawn,
                                     WinningResult& result) {
  uint8_t count    = 0;
  bool single_wait = false;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Eyes && tile_ids[0] == new_tile_id) {
      single_wait = true;
    }
    if (meld.GetMeldType() == MeldType::Triplet) {
      count++;
    }
  }
  for (auto&& meld : expose) {
    if (meld.GetMeldType() == MeldType::ConcealedKong) {
      count++;
    }
  }
  if (count != 4) {
    return;
  }

  if (single_wait) {
    result.AddYakuman(2);
    result.AddYaku(YakuType::SingleWaitFourConcealedTriplets);
  } else if (self_drawn) {
    result.AddYakuman(1);
    result.AddYaku(YakuType::FourConcealedTriplets);
  }
}

void AllHonors::TryMatch(const std::vector<MeldInId>& hand,
                         const std::vector<MeldInId>& expose,
                         WinningResult& result) {
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (!Tile::IsHonor(tile_ids[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (!Tile::IsHonor(tile_ids[0])) {
      return;
    }
  }

  result.AddYakuman(1);
  result.AddYaku(YakuType::AllHonors);
}

void AllGreen::TryMatch(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        WinningResult& result) {
  auto is_green = [](TileId id) -> bool {
    return id == _2s || id == _3s || id == _4s || id == _6s || id == _8s
           || id == F;
  };

  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (!is_green(tile_ids[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (!is_green(tile_ids[0])) {
      return;
    }
  }

  result.AddYakuman(1);
  result.AddYaku(YakuType::AllGreen);
}

void AllTerminals::TryMatch(const std::vector<MeldInId>& hand,
                            const std::vector<MeldInId>& expose,
                            WinningResult& result) {
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence || Tile::IsHonor(tile_ids[0])
        || !Tile::IsTerminal(tile_ids[0])) {
      return;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence || Tile::IsHonor(tile_ids[0])
        || !Tile::IsTerminal(tile_ids[0])) {
      return;
    }
  }

  result.AddYakuman(1);
  result.AddYaku(YakuType::AllTerminals);
}

void ThirteenOrphans::TryMatch(const Hand& hand,
                               TileId new_tile_id,
                               WinningResult& result) {
  std::unordered_set<TileId> tile_set;
  for (auto&& tile : hand.GetHands()) {
    if (TileId id = tile->GetId(); Tile::IsTerminal(id)) {
      tile_set.insert(id);
    }
  }
  if (tile_set.size() == 12 && tile_set.find(new_tile_id) == tile_set.end()) {
    result.AddYakuman(1);
    result.AddYaku(YakuType::ThirteenOrphans);
  } else if (tile_set.size() == 13) {
    result.AddYakuman(2);
    result.AddYaku(YakuType::ThirteenWaitThirteenOrphans);
  }
}

void FourWind::TryMatch(const std::vector<MeldInId>& hand,
                        const std::vector<MeldInId>& expose,
                        WinningResult& result) {
  uint8_t count = 0;
  bool wind_eye = false;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsWind(tile_ids[0])) {
      if (meld.GetMeldType() == MeldType::Eyes) {
        wind_eye = true;
      }
      count++;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsWind(tile_ids[0])) {
      count++;
    }
  }
  if (count == 3 && wind_eye) {
    result.AddYakuman(1);
    result.AddYaku(YakuType::SmallFourWinds);
  } else if (count == 4) {
    result.AddYakuman(2);
    result.AddYaku(YakuType::BigFourWinds);
  }
}

void FourQuads::TryMatch(const std::vector<MeldInId>& expose,
                         WinningResult& result) {
  uint8_t count = 0;
  for (auto&& meld : expose) {
    if (meld.GetMeldType() == MeldType::ConcealedKong
        || meld.GetMeldType() == MeldType::ExposeKong) {
      count++;
    }
  }
  if (count == 4) {
    result.AddYakuman(1);
    result.AddYaku(YakuType::FourQuads);
  }
}

void NineGates::TryMatch(const Hand& hand,
                         TileId new_tile_id,
                         WinningResult& result) {
  std::unordered_map<TileId, uint8_t> counts;
  uint8_t category = hand.GetHands().front()->GetId() / 10;

  for (auto&& tile : hand.GetHands()) {
    TileId id = tile->GetId();
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
  auto one  = static_cast<TileId>(category * 10 + 1);
  auto nine = static_cast<TileId>(category * 10 + 9);

  if (counts.size() < 9 || counts[one] < 3 || counts[nine] < 3) {
    return;
  }

  bool pure
      = ((new_tile_id == one || new_tile_id == nine) && counts[new_tile_id] > 3)
        || (counts[new_tile_id] > 1);

  if (pure) {
    result.AddYakuman(2);
    result.AddYaku(YakuType::PureNineGates);
  } else {
    result.AddYakuman(1);
    result.AddYaku(YakuType::NineGates);
  }
}
};  // namespace mahjong