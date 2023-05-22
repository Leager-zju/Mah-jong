#include <cstdint>

#include "Expose.hpp"
#include "Hand.hpp"
#include "OnePointYaku.hpp"
#include "TwoPointYaku.hpp"
#include "YakuMatcher.hpp"
#include "gtest/gtest.h"

using MAHJONG::Expose;
using MAHJONG::Hand;
using MAHJONG::Tile;
using MAHJONG::YakuMatcher;

TEST(Yaku, WhiteDragonYaku) {
  Hand hand;
  Expose expose;

  Tile has[13] = {Tile(MAHJONG::_1m), Tile(MAHJONG::_2m), Tile(MAHJONG::_3m),
                  Tile(MAHJONG::_4p), Tile(MAHJONG::_5p), Tile(MAHJONG::_6p),
                  Tile(MAHJONG::_7s), Tile(MAHJONG::_8s), Tile(MAHJONG::_9s),
                  Tile(MAHJONG::_B),  Tile(MAHJONG::_B),  Tile(MAHJONG::_B),
                  Tile(MAHJONG::_F)};
  for (size_t i = 0; i < 13; i++) {
    hand.draw(has + i);
  }
  Tile new_tile = Tile(MAHJONG::_F);

  auto&& res = YakuMatcher::detect(hand, expose, &new_tile, false, false);
  EXPECT_EQ(res.points_, 1);
  res.show_result();
}

TEST(Yaku, PureNineGates) {
  Hand hand;
  Expose expose;

  Tile has[13] = {Tile(MAHJONG::_1m), Tile(MAHJONG::_1m), Tile(MAHJONG::_1m),
                  Tile(MAHJONG::_2m), Tile(MAHJONG::_3m), Tile(MAHJONG::_4m),
                  Tile(MAHJONG::_5m), Tile(MAHJONG::_6m), Tile(MAHJONG::_7m),
                  Tile(MAHJONG::_8m), Tile(MAHJONG::_9m), Tile(MAHJONG::_9m),
                  Tile(MAHJONG::_9m)};
  for (size_t i = 0; i < 13; i++) {
    hand.draw(has + i);
  }
  Tile new_tile = Tile(MAHJONG::_5m);
  auto&& res = YakuMatcher::detect(hand, expose, &new_tile, false, false);
  EXPECT_EQ(res.yakuman_, 2);
  res.show_result();
}