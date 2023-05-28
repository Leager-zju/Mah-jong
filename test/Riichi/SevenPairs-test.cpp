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

TEST(SevenPairs, Test1) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_1m),
                  Tile(mahjong::_1m),
                  Tile(mahjong::_2m),
                  Tile(mahjong::_2m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::Z),
                  Tile(mahjong::Z),
                  Tile(mahjong::B),
                  Tile(mahjong::B),
                  Tile(mahjong::F),
                  Tile(mahjong::F),
                  Tile(mahjong::_4p),
                  Tile(mahjong::_5p)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  EXPECT_EQ(results.size(), 2);

  WaitingResult result1 = results[0];
  EXPECT_EQ(result1.GetDiscard(), mahjong::_5p);
  EXPECT_EQ(result1.GetWaitingFor(), std::set<TileId>{mahjong::_4p});

  WaitingResult result2 = results[1];
  EXPECT_EQ(result2.GetDiscard(), mahjong::_4p);
  EXPECT_EQ(result2.GetWaitingFor(), std::set<TileId>{mahjong::_5p});
}

TEST(SevenPairs, Test2) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_1m),
                  Tile(mahjong::_1m),
                  Tile(mahjong::_2m),
                  Tile(mahjong::_2m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::Z),
                  Tile(mahjong::Z),
                  Tile(mahjong::B),
                  Tile(mahjong::B),
                  Tile(mahjong::F),
                  Tile(mahjong::F),
                  Tile(mahjong::_5p),
                  Tile(mahjong::_5p)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  EXPECT_EQ(results.size(), 7);
}