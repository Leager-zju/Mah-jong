#include "YakusMatcher.hpp"

#include "Common.hpp"
#include "Expose.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "OnePointYaku.hpp"
#include "ThreePointYaku.hpp"
#include "TwoPointYaku.hpp"
#include "Yakuman.hpp"

#include <corecrt.h>

#include <cassert>
#include <vector>

namespace mahjong {

std::array<const char*, 47> yaku_in_string = {
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

void MatchResult::ShowResult() const {
  std::cout << '\n';
  for (YakuType yaku : yakus_matched_) {
    std::cout << mahjong::yakus_matcher::Yaku2String(yaku);
  }
  if (yakuman_) {
    std::cout << "------------------------------------------------\n";
    std::cout << "TOTAL_TILES                    ...           " << yakuman_
              << " YAKUMAN\n";
  } else if (points_) {
    if (dora_) {
      std::cout << "Dora                     ...             " << dora_
                << " Han\n";
    }
    if (inner_dora_) {
      std::cout << "InnerDora                ...             " << inner_dora_
                << " Han\n";
    }
    std::cout << "------------------------------------------------\n";
    auto total_point = points_ + dora_ + inner_dora_;
    std::cout << "TOTAL_TILES                    ...             " << total_point
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

MatchResult mahjong::yakus_matcher::TryAllYakuMatch(const Hand& hand,
                                           const Expose& expose,
                                           pTile new_tile,
                                           bool Riichi,
                                           bool self_drawn) {
  MatchResult result{};
  std::vector<MeldInId> hand_melds_in_id;
  std::vector<MeldInId> expose_melds_in_id;
  hand_melds_in_id.reserve(5);
  expose_melds_in_id.reserve(5);
  BuildMelds(hand, expose, new_tile, hand_melds_in_id, expose_melds_in_id);

  if (hand_melds_in_id.empty()) {
    return result;
  }

  TileId id = new_tile->GetId();

  bool menzenchin = true;
  for (auto&& meld : expose_melds_in_id) {
    if (meld.GetMeldType() != MeldType::ConcealedKong) {
      menzenchin = false;
      break;
    }
  }
  // 先判断役满
  BigThreeDragons::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  AllHonors::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  AllGreen::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  AllTerminals::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  FourWind::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  FourQuads::TryMatch(expose_melds_in_id, result);
  if (menzenchin) {
    FourConcealedTriplets::TryMatch(
        hand_melds_in_id, expose_melds_in_id, id, self_drawn, result);
    ThirteenOrphans::TryMatch(hand, id, result);
    NineGates::TryMatch(hand, id, result);
  }

  if (result.GetYakuman()) {
    return result;
  }

  if (Riichi) {
    assert(menzenchin);
    Riichi::TryMatch(result);
    Ippatsu::TryMatch(result);
    Tsumo::TryMatch(self_drawn, result);
  }
  if (menzenchin) {
    Pinfu::TryMatch(hand_melds_in_id, new_tile->GetId(), result);
    PureDoubleSequence::TryMatch(hand_melds_in_id, result);
    TwicePureDoubleSequence::TryMatch(hand_melds_in_id, result);
    if (!result.GetYakusMatched().empty()
        && result.GetYakusMatched().back()
               != YakuType::TwicePureDoubleSequence) {
      SevenPairs::TryMatch(hand, id, result);
    }
  }

  UnderTheSeaOrRiver::TryMatch(self_drawn, result);
  Wind::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  Dragon::TryMatch(hand_melds_in_id, expose_melds_in_id, result);

  OutsideHand::TryMatch(
      hand_melds_in_id, expose_melds_in_id, menzenchin, result);
  PureStraight::TryMatch(
      hand_melds_in_id, expose_melds_in_id, menzenchin, result);
  MixedTripleSequence::TryMatch(
      hand_melds_in_id, expose_melds_in_id, menzenchin, result);
  TripleTriplet::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  AllTriplet::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  ThreeConcealedTriplets::TryMatch(
      hand_melds_in_id, expose_melds_in_id, result);
  AllTerminalsAndHonors::TryMatch(hand_melds_in_id, expose_melds_in_id, result);
  Flush::TryMatch(hand_melds_in_id, expose_melds_in_id, menzenchin, result);
  return result;
}

namespace yakus_matcher {
  void BuildMelds(const Hand& hand,
                  const Expose& expose,
                  pTile new_tile,
                  std::vector<MeldInId>& hand_melds_in_id,
                  std::vector<MeldInId>& expose_melds_in_id) {
    // make melds
    std::unordered_map<TileId, size_t> table;
    for (auto&& tile : hand.GetHands()) {
      TileId id   = tile->GetId();
      auto&& iter = table.find(id);
      if (iter == table.end()) {
        table.emplace(id, 1);
      } else {
        iter->second++;
      }
    }
    TileId new_tile_id = new_tile->GetId();
    auto&& new_iter    = table.find(new_tile_id);
    if (new_iter == table.end()) {
      table.emplace(new_tile_id, 1);
    } else {
      new_iter->second++;
    }
    for (auto&& meld : expose.GetExposes()) {
      expose_melds_in_id.emplace_back(meld);
    }

    for (auto&& iter = table.begin(); iter != table.end(); iter++) {
      if (iter->second >= 2) {
        iter->second -= 2;
        hand_melds_in_id.emplace_back(MeldType::Eyes, iter->first, iter->first);
        if (FindMeld(table, hand_melds_in_id)) {
          break;
        }
        hand_melds_in_id.pop_back();
        iter->second += 2;
      }
    }
  }

  bool FindMeld(std::unordered_map<TileId, size_t>& table,
                std::vector<MeldInId>& melds) {
    bool clear = true;
    for (auto&& iter = table.begin(); iter != table.end(); iter++) {
      if (iter->second) {
        clear = false;
      }
      if (iter->second >= 3) {
        iter->second -= 3;
        melds.emplace_back(
            MeldType::Triplet, iter->first, iter->first, iter->first);
        if (FindMeld(table, melds)) {
          return true;
        }
        melds.pop_back();
        iter->second += 3;
      }

      if (iter->second >= 1) {
        auto&& iter_plus1 = table.find(static_cast<TileId>(iter->first + 1));
        auto&& iter_plus2 = table.find(static_cast<TileId>(iter->first + 2));
        if (iter_plus1 == table.end() || iter_plus1->second == 0
            || iter_plus2 == table.end() || iter_plus2->second == 0) {
          continue;
        }
        iter->second--;
        iter_plus1->second--;
        iter_plus2->second--;
        melds.emplace_back(MeldType::Sequence,
                           iter->first,
                           iter_plus1->first,
                           iter_plus2->first);
        if (FindMeld(table, melds)) {
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

  const char* Yaku2String(YakuType yaku) {
    return yaku_in_string.at(static_cast<const size_t>(yaku));
  }
};  // namespace yakus_matcher
};  // namespace mahjong