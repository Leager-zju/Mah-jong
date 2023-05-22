#include "OnePointYaku.hpp"

#include <unordered_set>

#include "Common.hpp"
#include "Meld.hpp"
#include "Tiles.hpp"
#include "YakuMatcher.hpp"

namespace MAHJONG {
void Tsumo::try_match(bool self_drawn, MatchResult& result) {
  if (self_drawn) {
    result.points_ += 1;
    result.Yakus_matched_.push_back(YakuType::Tsumo);
  }
}

void Riichi::try_match(MatchResult& result) {
  result.points_++;
  result.Yakus_matched_.push_back(YakuType::Riichi);
}

void UnderTheSeaOrRiver::try_match(bool self_drawn, MatchResult& result) {
  auto&& global_tile_manager_ = GlobalTileManager::get_tile_manager();
  if (global_tile_manager_->is_empty()) {
    result.points_ += 1;
    result.Yakus_matched_.push_back(self_drawn ? YakuType::UnderTheSea
                                               : YakuType::UnderTheRiver);
  }
}

void Wind::try_match(const std::vector<MeldInId>& hand,
                     const std::vector<MeldInId>& expose,
                     const std::unique_ptr<MajManager>& maj_manager,
                     MatchResult& result) {
  // 大四喜->小四喜->自风/场风
  uint8_t east_triplet = 0;
  uint8_t south_triplet = 0;
  uint8_t west_triplet = 0;
  uint8_t north_triplet = 0;

  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Triplet) {
      if (meld.tile_ids_[0] == _E) {
        east_triplet = 1;
      } else if (meld.tile_ids_[0] == _S) {
        south_triplet = 1;
      } else if (meld.tile_ids_[0] == _W) {
        west_triplet = 1;
      } else if (meld.tile_ids_[0] == _N) {
        north_triplet = 1;
      }
    }
  }

  for (auto&& meld : expose) {
    if (meld.meld_type_ != MeldType::Sequence) {  // 刻/杠
      if (meld.tile_ids_[0] == _E) {
        east_triplet = 1;
      } else if (meld.tile_ids_[0] == _S) {
        south_triplet = 1;
      } else if (meld.tile_ids_[0] == _W) {
        west_triplet = 1;
      } else if (meld.tile_ids_[0] == _N) {
        north_triplet = 1;
      }
    }
  }

  uint16_t seat_wind = maj_manager->get_seat_wind();
  uint16_t banker_index = maj_manager->get_banker_index();
  uint16_t player_index = maj_manager->get_my_player_index();
  uint16_t prevalent_wind = (player_index - banker_index + 4) % 4;

  if ((seat_wind == 0 && east_triplet) || (seat_wind == 1 && south_triplet) ||
      (seat_wind == 2 && west_triplet) || (seat_wind == 3 && north_triplet)) {
    result.points_ += 1;
    result.Yakus_matched_.push_back(YakuType::SeatWind);
  }
  if ((prevalent_wind == 0 && east_triplet) ||
      (prevalent_wind == 1 && south_triplet) ||
      (prevalent_wind == 2 && west_triplet) ||
      (prevalent_wind == 3 && north_triplet)) {
    result.points_ += 1;
    result.Yakus_matched_.push_back(YakuType::PrevalentWind);
  }
}

void Dragon::try_match(const std::vector<MeldInId>& hand,
                       const std::vector<MeldInId>& expose,
                       MatchResult& result) {
  uint8_t red_triplet = 0;
  uint8_t white_triplet = 0;
  uint8_t green_triplet = 0;
  uint8_t dragon_eyes = 0;

  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Triplet) {
      if (meld.tile_ids_[0] == _Z) {
        red_triplet = 1;
      } else if (meld.tile_ids_[0] == _B) {
        white_triplet = 1;
      } else if (meld.tile_ids_[0] == _F) {
        green_triplet = 1;
      }
    }

    if (meld.meld_type_ == MeldType::Eyes) {
      if (meld.tile_ids_[0] == _Z || meld.tile_ids_[0] == _B ||
          meld.tile_ids_[0] == _F) {
        dragon_eyes = 1;
      }
    }
  }

  for (auto&& meld : expose) {
    if (meld.meld_type_ != MeldType::Sequence) {  // 刻/杠
      if (meld.tile_ids_[0] == _Z) {
        red_triplet = 1;
      } else if (meld.tile_ids_[0] == _B) {
        white_triplet = 1;
      } else if (meld.tile_ids_[0] == _F) {
        green_triplet = 1;
      }
    }
  }

  if (red_triplet + white_triplet + green_triplet == 2 && dragon_eyes) {
    result.points_ += 2;
    result.Yakus_matched_.push_back(YakuType::SmallThreeDragons);
    return;
  }

  if (red_triplet) {
    result.points_ += 1;
    result.Yakus_matched_.push_back(YakuType::RedDragon);
  }
  if (white_triplet) {
    result.points_ += 1;
    result.Yakus_matched_.push_back(YakuType::WhiteDragon);
  }
  if (green_triplet) {
    result.points_ += 1;
    result.Yakus_matched_.push_back(YakuType::GreenDragon);
  }
}

void AllSimple::try_match(const std::vector<MeldInId>& hand,
                          const std::vector<MeldInId>& expose,
                          MatchResult& result) {
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence) {
      if (Tile::is_terminal(meld.tile_ids_[0]) ||
          Tile::is_terminal(meld.tile_ids_[2])) {
        return;
      }
    } else {
      if (Tile::is_terminal(meld.tile_ids_[0])) {
        return;
      }
    }
  }
  for (auto&& meld : expose) {
    if (meld.meld_type_ == MeldType::Sequence) {
      if (Tile::is_terminal(meld.tile_ids_[0]) ||
          Tile::is_terminal(meld.tile_ids_[2])) {
        return;
      }
    } else {
      if (Tile::is_terminal(meld.tile_ids_[0])) {
        return;
      }
    }
  }

  result.points_ += 1;
  result.Yakus_matched_.push_back(YakuType::AllSimple);
}

void PureDoubleSequence::try_match(const std::vector<MeldInId>& hand,
                                   const std::vector<MeldInId>& expose,
                                   MatchResult& result) {
  assert(expose.empty());
  std::unordered_set<tile_id> occured;
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence) {
      if (occured.count(meld.tile_ids_[0])) {
        result.points_ += 1;
        result.Yakus_matched_.push_back(YakuType::PureDoubleSequence);
        return;
      }
      occured.insert(meld.tile_ids_[0]);
    }
  }
}

void Pinfu::try_match(const std::vector<MeldInId>& hand, tile_id new_tile_id,
                      MatchResult& result) {
  for (auto&& meld : hand) {
    if (meld.meld_type_ == MeldType::Sequence) {
      if (meld.tile_ids_[0] == new_tile_id &&
          !Tile::is_terminal(meld.tile_ids_[2])) {
        result.points_ += 1;
        result.Yakus_matched_.push_back(YakuType::Pinfu);
        return;
      }

      if (meld.tile_ids_[2] == new_tile_id &&
          !Tile::is_terminal(meld.tile_ids_[1])) {
        result.points_ += 1;
        result.Yakus_matched_.push_back(YakuType::Pinfu);
        return;
      }
    }
  }
}

void Ippatsu::try_match(MatchResult& result) {
  // TODO(Leager)
}
};  // namespace MAHJONG