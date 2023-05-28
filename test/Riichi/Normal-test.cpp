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

TEST(Normal, Test1) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_2m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_4m),
                  Tile(mahjong::_5m),
                  Tile(mahjong::_6m),
                  Tile(mahjong::_2s),
                  Tile(mahjong::_3s),
                  Tile(mahjong::_4s),
                  Tile(mahjong::_5s),
                  Tile(mahjong::_6s),
                  Tile(mahjong::_7s),
                  Tile(mahjong::_5p),
                  Tile(mahjong::_5p),
                  Tile(mahjong::F)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  for (auto&& result : results) {
    std::cout << result.GetDiscard() << '\n';
  }

  EXPECT_EQ(results.size(), 1);

  WaitingResult result1 = results[0];
  std::set<TileId> waiting_for{mahjong::_1m, mahjong::_4m, mahjong::_7m};
  EXPECT_EQ(result1.GetDiscard(), mahjong::F);
  EXPECT_EQ(result1.GetWaitingFor(), waiting_for);
}

TEST(Normal, Test2) {
  Hand hand;
  Expose expose;

  Tile has[14] = {Tile(mahjong::_2m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_3m),
                  Tile(mahjong::_4m),
                  Tile(mahjong::_4m),
                  Tile(mahjong::_7m),
                  Tile(mahjong::_8m),
                  Tile(mahjong::_9m),
                  Tile(mahjong::_1s),
                  Tile(mahjong::_2s),
                  Tile(mahjong::_3s),
                  Tile(mahjong::_6p),
                  Tile(mahjong::_7p),
                  Tile(mahjong::_8p)};
  for (auto&& tile : has) {
    hand.Draw(&tile);
  }

  auto&& results = mahjong::riichi_detector::CheckRiichi(hand, expose);
  for (auto&& result : results) {
    std::cout << result.GetDiscard() << '\n';
  }

  EXPECT_EQ(results.size(), 3);

  WaitingResult result1 = results[0];
  std::set<TileId> waiting_for1{mahjong::_3m, mahjong::_4m};
  EXPECT_EQ(result1.GetDiscard(), mahjong::_2m);
  EXPECT_EQ(result1.GetWaitingFor(), waiting_for1);

  WaitingResult result2 = results[1];
  std::set<TileId> waiting_for2{mahjong::_1m, mahjong::_4m};
  EXPECT_EQ(result2.GetDiscard(), mahjong::_3m);
  EXPECT_EQ(result2.GetWaitingFor(), waiting_for2);

  WaitingResult result3 = results[2];
  std::set<TileId> waiting_for3{mahjong::_3m};
  EXPECT_EQ(result3.GetDiscard(), mahjong::_4m);
  EXPECT_EQ(result3.GetWaitingFor(), waiting_for3);
}