#include "Expose.hpp"
#include "Hand.hpp"
#include "MajManager.hpp"
#include "Meld.hpp"
#include "OnePointYaku.hpp"
#include "TwoPointYaku.hpp"
#include "YakusMatcher.hpp"
#include "gtest/gtest.h"

#include <cstdint>

using mahjong::Expose;
using mahjong::Hand;
using mahjong::Meld;
using mahjong::Tile;
using mahjong::yakus_matcher::TryAllYakuMatch;

TEST(Yaku, WhiteDragonYaku) {
  Hand hand;
  Expose expose;

  Tile has[13] = {Tile(mahjong::_1m),
                  Tile(mahjong::_2m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_4p),
                  Tile(mahjong::_5p),
                  Tile(mahjong::_6p),
                  Tile(mahjong::_7s),
                  Tile(mahjong::_8s),
                  Tile(mahjong::_9s),
                  Tile(mahjong::B),
                  Tile(mahjong::B),
                  Tile(mahjong::B),
                  Tile(mahjong::F)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }
  Tile new_tile = Tile(mahjong::F);

  auto&& res = TryAllYakuMatch(hand, expose, &new_tile, false, false);
  EXPECT_EQ(res.GetPoint(), 1);
  res.ShowResult();
}

TEST(Yaku, PureNineGates) {
  Hand hand;
  Expose expose;

  Tile has[13] = {Tile(mahjong::_1m),
                  Tile(mahjong::_1m),
                  Tile(mahjong::_1m),
                  Tile(mahjong::_2m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_4m),
                  Tile(mahjong::_5m),
                  Tile(mahjong::_6m),
                  Tile(mahjong::_7m),
                  Tile(mahjong::_8m),
                  Tile(mahjong::_9m),
                  Tile(mahjong::_9m),
                  Tile(mahjong::_9m)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }
  Tile new_tile = Tile(mahjong::_5m);
  auto&& res    = mahjong::yakus_matcher::TryAllYakuMatch(
      hand, expose, &new_tile, false, false);
  EXPECT_EQ(res.GetYakuman(), 2);
  res.ShowResult();
}

TEST(Yaku, Normal) {
  Hand hand;
  Expose expose;

  Tile has[10] = {Tile(mahjong::_2m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_4m),
                  Tile(mahjong::_3p),
                  Tile(mahjong::_4p),
                  Tile(mahjong::_5p),
                  Tile(mahjong::_7p),
                  Tile(mahjong::_8p),
                  Tile(mahjong::_5s),
                  Tile(mahjong::_5s)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  Tile melds[3] = {Tile(mahjong::_6s), Tile(mahjong::_6s), Tile(mahjong::_6s)};
  expose.AddNewMeld(
      Meld(mahjong::MeldType::Triplet, &melds[0], &melds[1], &melds[2]));

  Tile new_tile  = Tile(mahjong::_6p);
  auto&& manager = mahjong::MajManager::GetMajManager();
  auto&& res = mahjong::yakus_matcher::TryAllYakuMatch(
      hand, expose, &new_tile, false, false);
  EXPECT_TRUE(res.HasResult());
  res.ShowResult();
}