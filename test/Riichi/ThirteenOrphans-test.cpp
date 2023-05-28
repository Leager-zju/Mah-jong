#include "Common.hpp"
#include "Expose.hpp"
#include "Hand.hpp"
#include "RiichiDetector.hpp"
#include "gtest/gtest.h"

#include <cstdint>

using mahjong::Expose;
using mahjong::Hand;
using mahjong::Tile;
using mahjong::TileId;
using mahjong::WaitingResult;

const std::set<TileId> ORPHANS{mahjong::_1m,
                               mahjong::_9m,
                               mahjong::_1p,
                               mahjong::_9p,
                               mahjong::_1s,
                               mahjong::_9s,
                               mahjong::E,
                               mahjong::S,
                               mahjong::W,
                               mahjong::N,
                               mahjong::Z,
                               mahjong::B,
                               mahjong::F};

TEST(ThirteenOrphans, Test1) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_1m),
                  Tile(mahjong::_9m),
                  Tile(mahjong::_1p),
                  Tile(mahjong::_9p),
                  Tile(mahjong::_1s),
                  Tile(mahjong::_9s),
                  Tile(mahjong::E),
                  Tile(mahjong::S),
                  Tile(mahjong::W),
                  Tile(mahjong::N),
                  Tile(mahjong::Z),
                  Tile(mahjong::B),
                  Tile(mahjong::B),
                  Tile(mahjong::_5p)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  EXPECT_EQ(results.size(), 1);

  WaitingResult result = results[0];
  EXPECT_EQ(result.GetDiscard(), mahjong::_5p);
  EXPECT_EQ(result.GetWaitingFor(), std::set<TileId>{mahjong::F});
}

TEST(ThirteenOrphans, Test2) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_1m),
                  Tile(mahjong::_9m),
                  Tile(mahjong::_1p),
                  Tile(mahjong::_9p),
                  Tile(mahjong::_1s),
                  Tile(mahjong::_9s),
                  Tile(mahjong::E),
                  Tile(mahjong::S),
                  Tile(mahjong::W),
                  Tile(mahjong::N),
                  Tile(mahjong::Z),
                  Tile(mahjong::B),
                  Tile(mahjong::F),
                  Tile(mahjong::_5p)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  EXPECT_EQ(results.size(), 1);

  WaitingResult result = results[0];

  EXPECT_EQ(result.GetDiscard(), mahjong::_5p);
  EXPECT_EQ(result.GetWaitingFor(), ORPHANS);
}

TEST(ThirteenOrphans, Test3) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_1m),
                  Tile(mahjong::_9m),
                  Tile(mahjong::_1p),
                  Tile(mahjong::_9p),
                  Tile(mahjong::_1s),
                  Tile(mahjong::_9s),
                  Tile(mahjong::E),
                  Tile(mahjong::S),
                  Tile(mahjong::W),
                  Tile(mahjong::N),
                  Tile(mahjong::Z),
                  Tile(mahjong::B),
                  Tile(mahjong::F),
                  Tile(mahjong::_1m)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }
  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  EXPECT_EQ(results.size(), 13);

  WaitingResult result = results[0];
  EXPECT_EQ(result.GetDiscard(), mahjong::_1m);
  EXPECT_EQ(result.GetWaitingFor(), ORPHANS);
}

TEST(ThirteenOrphans, Test4) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_1m),
                  Tile(mahjong::_9m),
                  Tile(mahjong::_1p),
                  Tile(mahjong::_9p),
                  Tile(mahjong::_1s),
                  Tile(mahjong::_9s),
                  Tile(mahjong::E),
                  Tile(mahjong::S),
                  Tile(mahjong::W),
                  Tile(mahjong::N),
                  Tile(mahjong::Z),
                  Tile(mahjong::B),
                  Tile(mahjong::B),
                  Tile(mahjong::B)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  EXPECT_TRUE(results.empty());
}