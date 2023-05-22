#include "YakuMatcher.hpp"

#include <corecrt.h>

#include <cassert>
#include <vector>

#include "Common.hpp"
#include "Expose.hpp"
#include "GlobalTileManager.hpp"
#include "Hand.hpp"
#include "MajManager.hpp"
#include "Meld.hpp"
#include "OnePointYaku.hpp"
#include "ThreePointYaku.hpp"
#include "TwoPointYaku.hpp"
#include "Yakuman.hpp"

namespace MAHJONG {

const char* YakuInString[] = {
    "Tsumo                    ...             1 Han\n",
    "Riichi                   ...             1 Han\n",
    "UnderTheSea              ...             1 Han\n",
    "UnderTheRiver            ...             1 Han\n",
    "SeatWind                 ...             1 Han\n",
    "PrevalentWind            ...             1 Han\n",
    "RedDragon                ...             1 Han\n",
    "WhiteDragon              ...             1 Han\n",
    "GreenDragon              ...             1 Han\n",
    "AllSimple                ...             1 Han\n",
    "PureDoubleSequence       ...             1 Han\n",
    "Pinfu                    ...             1 Han\n",

    "HalfOutsideHand          ...             2 Han\n",  // Menzenchin
    "HalfOutsideHand          ...             1 Han\n",  // Expose
    "PureStraight             ...             2 Han\n",  // Menzenchin
    "PureStraight             ...             1 Han\n",  // Expose
    "MixedTripleSequence      ...             2 Han\n",  // Menzenchin
    "MixedTripleSequence      ...             1 Han\n",  // Expose
    "TripleTriplet            ...             2 Han\n",
    "ThreeQuads               ...             2 Han\n",
    "AllTriplet               ...             2 Han\n",
    "ThreeConcealedTriplets   ...             2 Han\n",
    "AllTerminalsAndHonors    ...             2 Han\n",
    "SevenPairs               ...             2 Han\n",
    "SmallThreeDragons        ...             2 Han\n",

    "FullyOutsideHand         ...             3 Han\n",  // Menzenchin
    "FullyOutsideHand         ...             2 Han\n",  // Expose
    "HalfFlush                ...             3 Han\n",  // Menzenchin
    "HalfFlush                ...             2 Han\n",  // Expose
    "TwicePureDoubleSequence  ...             3 Han\n",

    "FullyFlush               ...             6 Han\n",  // Menzenchin
    "FullyFlush               ...             5 Han\n",  // Expose

    "BlessingOfHeaven !!!\n",
    "BlessingOfEarth !!!\n",
    "BigThreeDragons !!!\n",
    "FourConcealedTriplets !!!\n",
    "AllHonors !!!\n",
    "AllGreen !!!\n",
    "AllTerminals !!!\n",
    "ThirteenOrphans !!!\n",
    "SmallFourWinds !!!\n",
    "FourQuads !!!\n",
    "NineGates !!!\n",

    "PureNineGates !!!\n",
    "SingleWaitFourConcealedTriplets !!!\n",
    "ThirteenWaitThirteenOrphans !!!\n",
    "BigFourWinds !!!\n",
};

void MatchResult::show_result() const {
  std::cout << '\n';
  for (YakuType yaku : Yakus_matched_) {
    std::cout << YakuMatcher::Yaku2String(yaku);
  }
  if (yakuman_) {
    std::cout << "------------------------------------------------\n";
    std::cout << "TOTAL                    ...           " << yakuman_ << " YAKUMAN\n";
  } else if (points_) {
    if (dora_) {
      std::cout << "Dora                     ...             " << dora_
                << " Han\n";
    }
    if (inner_dora_) {
      std::cout << "InnerDora                ...             "
                << inner_dora_ << " Han\n";
    }
    std::cout << "------------------------------------------------\n";
    auto total_point = points_ + dora_ + inner_dora_;
    std::cout << "TOTAL                    ...             " << total_point
              << " Han\n";
    if (total_point >= 13) {
      std::cout << "KAZOE YAKUMAN!\n";
    } else if (total_point >= 11) {
      std::cout << "SANBAIMAN!\n";
    } else if (total_point >= 8) {
      std::cout << "BAIMAN\n";
    } else if (total_point >= 6) {
      std::cout << "HANEMAN!\n";
    } else if (total_point >= 5) {
      std::cout << "MANGAN!\n";
    }
  }
  std::cout << '\n';
}

MatchResult YakuMatcher::detect(const Hand& hand, const Expose& expose,
                                pTile new_tile, bool Riichi, bool self_drawn) {
  std::cout << "Detect...\n";
  hand.show();
  expose.show();
  std::cout << "New Tile: " << new_tile->to_string() << '\n';

  return try_all_yaku_match(hand, expose, new_tile, Riichi, self_drawn);
}

MatchResult YakuMatcher::try_all_yaku_match(const Hand& hand,
                                            const Expose& expose,
                                            pTile new_tile, bool Riichi,
                                            bool self_drawn) {
  std::vector<MeldInId> hand_melds_in_id;
  std::vector<MeldInId> expose_melds_in_id;
  hand_melds_in_id.reserve(5);
  expose_melds_in_id.reserve(5);
  build_melds(hand, expose, new_tile, hand_melds_in_id, expose_melds_in_id);

  if (hand_melds_in_id.empty()) {
    return {};
  }

  tile_id id = new_tile->to_id();
  MatchResult result;
  bool Menzenchin = true;
  for (auto&& meld : expose_melds_in_id) {
    if (meld.meld_type_ != MeldType::ConcealedKong) {
      Menzenchin = false;
      break;
    }
  }
  // 先判断役满
  BigThreeDragons::try_match(hand_melds_in_id, expose_melds_in_id, result);
  AllHonors::try_match(hand_melds_in_id, expose_melds_in_id, result);
  AllGreen::try_match(hand_melds_in_id, expose_melds_in_id, result);
  AllTerminals::try_match(hand_melds_in_id, expose_melds_in_id, result);
  FourWind::try_match(hand_melds_in_id, expose_melds_in_id, result);
  FourQuads::try_match(hand_melds_in_id, expose_melds_in_id, result);
  if (Menzenchin) {
    FourConcealedTriplets::try_match(hand_melds_in_id, expose_melds_in_id, id,
                                     self_drawn, result);
    ThirteenOrphans::try_match(hand, id, result);
    NineGates::try_match(hand, id, result);
  }

  if (result.yakuman_) {
    return result;
  }

  if (Riichi) {
    assert(Menzenchin);
    Riichi::try_match(result);
    Ippatsu::try_match(result);
    Tsumo::try_match(self_drawn, result);
  }
  if (Menzenchin) {
    Pinfu::try_match(hand_melds_in_id, new_tile->to_id(), result);
    PureDoubleSequence::try_match(hand_melds_in_id, expose_melds_in_id, result);
    Flush::try_match(hand_melds_in_id, expose_melds_in_id, Menzenchin, result);
    TwicePureDoubleSequence::try_match(hand_melds_in_id, result);
    if (!result.Yakus_matched_.empty() &&
        result.Yakus_matched_.back() != YakuType::TwicePureDoubleSequence) {
      SevenPairs::try_match(hand, id, result);
    }
  }

  UnderTheSeaOrRiver::try_match(self_drawn, result);
  Wind::try_match(hand_melds_in_id, expose_melds_in_id,
                  MajManager::get_maj_manager(), result);
  Dragon::try_match(hand_melds_in_id, expose_melds_in_id, result);

  OutsideHand::try_match(hand_melds_in_id, expose_melds_in_id, Menzenchin,
                         result);
  PureStraight::try_match(hand_melds_in_id, expose_melds_in_id, Menzenchin,
                          result);
  MixedTripleSequence::try_match(hand_melds_in_id, expose_melds_in_id,
                                 Menzenchin, result);
  TripleTriplet::try_match(hand_melds_in_id, expose_melds_in_id, result);
  AllTriplet::try_match(hand_melds_in_id, expose_melds_in_id, result);
  ThreeConcealedTriplets::try_match(hand_melds_in_id, expose_melds_in_id,
                                    result);
  AllTerminalsAndHonors::try_match(hand_melds_in_id, expose_melds_in_id,
                                   result);

  return result;
}

void YakuMatcher::build_melds(const Hand& hand, const Expose& expose,
                              pTile new_tile,
                              std::vector<MeldInId>& hand_melds_in_id,
                              std::vector<MeldInId>& expose_melds_in_id) {
  // make melds
  std::unordered_map<tile_id, size_t> table;
  for (auto&& tile : hand.get_hands()) {
    tile_id id = tile->to_id();
    auto&& iter = table.find(id);
    if (iter == table.end()) {
      table.emplace(id, 1);
    } else {
      iter->second++;
    }
  }
  tile_id new_tile_id = new_tile->to_id();
  auto&& new_iter = table.find(new_tile_id);
  if (new_iter == table.end()) {
    table.emplace(new_tile_id, 1);
  } else {
    new_iter->second++;
  }
  for (auto&& meld : expose.get_exposes()) {
    expose_melds_in_id.push_back(MeldInId(meld));
  }

  for (auto&& iter = table.begin(); iter != table.end(); iter++) {
    if (iter->second >= 2) {
      iter->second -= 2;
      hand_melds_in_id.push_back(
          MeldInId(MeldType::Eyes, iter->first, iter->first));
      if (find_meld(table, hand_melds_in_id)) {
        break;
      }
      hand_melds_in_id.pop_back();
      iter->second += 2;
    }
  }
}

bool YakuMatcher::find_meld(std::unordered_map<tile_id, size_t>& table,
                            std::vector<MeldInId>& melds) {
  bool clear = true;
  for (auto&& iter = table.begin(); iter != table.end(); iter++) {
    if (iter->second) {
      clear = false;
    }
    if (iter->second >= 3) {
      iter->second -= 3;
      melds.push_back(
          MeldInId(MeldType::Triplet, iter->first, iter->first, iter->first));
      if (find_meld(table, melds)) {
        return true;
      }
      melds.pop_back();
      iter->second += 3;
    }

    if (iter->second >= 1) {
      auto&& iter_plus1 = table.find(static_cast<tile_id>(iter->first + 1));
      auto&& iter_plus2 = table.find(static_cast<tile_id>(iter->first + 2));
      if (iter_plus1 == table.end() || iter_plus1->second == 0 ||
          iter_plus2 == table.end() || iter_plus2->second == 0) {
        continue;
      }
      iter->second--;
      iter_plus1->second--;
      iter_plus2->second--;
      melds.push_back(MeldInId(MeldType::Sequence, iter->first,
                               iter_plus1->first, iter_plus2->first));
      if (find_meld(table, melds)) {
        return true;
      }
      melds.pop_back();
      iter->second++;
      iter_plus1->second++;
      iter_plus2->second++;
    }
  }

  return clear;
}

const char* YakuMatcher::Yaku2String(YakuType yaku) {
  return YakuInString[static_cast<size_t>(yaku)];
}
};  // namespace MAHJONG