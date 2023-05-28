#include "OnePointYaku.hpp"

#include "Common.hpp"
#include "GlobalTileManager.hpp"
#include "MajManager.hpp"
#include "Meld.hpp"
#include "YakusMatcher.hpp"

#include <unordered_set>

namespace mahjong {
void Tsumo::TryMatch(bool self_drawn, WinningResult& result) {
  if (self_drawn) {
    result.AddPoint(1);
    result.AddYaku(YakuType::Tsumo);
  }
}

void Riichi::TryMatch(WinningResult& result) {
  result.AddPoint(1);
  result.AddYaku(YakuType::Riichi);
}

void UnderTheSeaOrRiver::TryMatch(bool self_drawn, WinningResult& result) {
  auto&& global_tile_manager = GlobalTileManager::GetTileManager();
  if (global_tile_manager->Empty()) {
    result.AddPoint(1);
    result.AddYaku(self_drawn ? YakuType::UnderTheSea
                              : YakuType::UnderTheRiver);
  }
}

void Wind::TryMatch(const std::vector<MeldInId>& hand,
                    const std::vector<MeldInId>& expose,
                    WinningResult& result) {
  auto&& maj_manager = MajManager::GetMajManager();
  // 大四喜->小四喜->自风/场风
  uint8_t east_triplet  = 0;
  uint8_t south_triplet = 0;
  uint8_t west_triplet  = 0;
  uint8_t north_triplet = 0;

  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Triplet) {
      if (tile_ids[0] == E) {
        east_triplet = 1;
      } else if (tile_ids[0] == S) {
        south_triplet = 1;
      } else if (tile_ids[0] == W) {
        west_triplet = 1;
      } else if (tile_ids[0] == N) {
        north_triplet = 1;
      }
    }
  }

  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() != MeldType::Sequence) {  // 刻/杠
      if (tile_ids[0] == E) {
        east_triplet = 1;
      } else if (tile_ids[0] == S) {
        south_triplet = 1;
      } else if (tile_ids[0] == W) {
        west_triplet = 1;
      } else if (tile_ids[0] == N) {
        north_triplet = 1;
      }
    }
  }

  uint16_t seat_wind      = maj_manager->GetSeatWind();
  uint16_t banker_index   = maj_manager->GetBankerIndex();
  uint16_t player_index   = maj_manager->GetMyPlayerIndex();
  uint16_t prevalent_wind = (player_index - banker_index + 4) % 4;

  if ((seat_wind == 0 && east_triplet) || (seat_wind == 1 && south_triplet)
      || (seat_wind == 2 && west_triplet)
      || (seat_wind == 3 && north_triplet)) {
    result.AddPoint(1);
    result.AddYaku(YakuType::SeatWind);
  }
  if ((prevalent_wind == 0 && east_triplet)
      || (prevalent_wind == 1 && south_triplet)
      || (prevalent_wind == 2 && west_triplet)
      || (prevalent_wind == 3 && north_triplet)) {
    result.AddPoint(1);
    result.AddYaku(YakuType::PrevalentWind);
  }
}

void Dragon::TryMatch(const std::vector<MeldInId>& hand,
                      const std::vector<MeldInId>& expose,
                      WinningResult& result) {
  uint8_t red_triplet   = 0;
  uint8_t white_triplet = 0;
  uint8_t green_triplet = 0;
  uint8_t dragon_eyes   = 0;

  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Triplet) {
      if (tile_ids[0] == Z) {
        red_triplet = 1;
      } else if (tile_ids[0] == B) {
        white_triplet = 1;
      } else if (tile_ids[0] == F) {
        green_triplet = 1;
      }
    }

    if (meld.GetMeldType() == MeldType::Eyes && Tile::IsDragon(tile_ids[0])) {
      dragon_eyes = 1;
    }
  }

  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() != MeldType::Sequence) {  // 刻/杠
      if (tile_ids[0] == Z) {
        red_triplet = 1;
      } else if (tile_ids[0] == B) {
        white_triplet = 1;
      } else if (tile_ids[0] == F) {
        green_triplet = 1;
      }
    }
  }

  if (red_triplet + white_triplet + green_triplet == 2 && dragon_eyes) {
    result.AddPoint(2);
    result.AddYaku(YakuType::SmallThreeDragons);
    return;
  }

  if (red_triplet) {
    result.AddPoint(1);
    result.AddYaku(YakuType::RedDragon);
  }
  if (white_triplet) {
    result.AddPoint(1);
    result.AddYaku(YakuType::WhiteDragon);
  }
  if (green_triplet) {
    result.AddPoint(1);
    result.AddYaku(YakuType::GreenDragon);
  }
}

void AllSimple::TryMatch(const std::vector<MeldInId>& hand,
                         const std::vector<MeldInId>& expose,
                         WinningResult& result) {
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsTerminal(tile_ids[0])
        || (meld.GetMeldType() == MeldType::Sequence
            && Tile::IsTerminal(tile_ids[2]))) {
      return;
    }
  }
  for (auto&& meld : expose) {
    auto&& tile_ids = meld.GetTileId();
    if (Tile::IsTerminal(tile_ids[0])
        || (meld.GetMeldType() == MeldType::Sequence
            && Tile::IsTerminal(tile_ids[2]))) {
      return;
    }
  }

  result.AddPoint(1);
  result.AddYaku(YakuType::AllSimple);
}

void PureDoubleSequence::TryMatch(const std::vector<MeldInId>& hand,
                                  WinningResult& result) {
  std::unordered_set<TileId> occured;
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence) {
      if (occured.count(tile_ids[0])) {
        result.AddPoint(1);
        result.AddYaku(YakuType::PureDoubleSequence);
        return;
      }
      occured.insert(tile_ids[0]);
    }
  }
}

void Pinfu::TryMatch(const std::vector<MeldInId>& hand,
                     TileId new_tile_id,
                     WinningResult& result) {
  for (auto&& meld : hand) {
    auto&& tile_ids = meld.GetTileId();
    if (meld.GetMeldType() == MeldType::Sequence) {
      if (tile_ids[0] == new_tile_id && !Tile::IsTerminal(tile_ids[2])) {
        result.AddPoint(1);
        result.AddYaku(YakuType::Pinfu);
        return;
      }

      if (tile_ids[2] == new_tile_id && !Tile::IsTerminal(tile_ids[1])) {
        result.AddPoint(1);
        result.AddYaku(YakuType::Pinfu);
        return;
      }
    }
  }
}

void Ippatsu::TryMatch(WinningResult& result) {
  // TODO(Leager)
}
};  // namespace mahjong