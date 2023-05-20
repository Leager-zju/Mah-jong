#include <corecrt.h>
#include <stdlib.h>

#include <cstddef>
#include <list>
#include <vector>

#include "Expose.hpp"
#include "Hand.hpp"
#include "Meld.hpp"
#include "TileTypes.hpp"
#include "Tiles.hpp"
#include "WinningDetector.hpp"
#include "gtest/gtest.h"

// 九莲宝灯
TEST(WinDetector, NineGatesTest) {
  srand(static_cast<unsigned int>(time(nullptr)));
  Hand hand;
  Expose expose;

  Tile NineGates[13] = {
      Tile(_1m), Tile(_1m), Tile(_1m), Tile(_2m), Tile(_3m), Tile(_4m), Tile(_5m),
      Tile(_6m), Tile(_7m), Tile(_8m), Tile(_9m), Tile(_9m), Tile(_9m),
  };
  for (size_t i = 0; i < 13; i++) {
    hand.draw(NineGates + i);
  }
  Tile new_tile = Tile(static_cast<tile_id>(std::rand() % 9 + 1));

  auto&& res = WinningDetector::detect(hand, expose, &new_tile);
  EXPECT_TRUE(res.has_result());
}

TEST(WinDetector, NormalTest) {
  Hand hand;
  Expose expose;

  Tile Has[7] = {Tile(_1m), Tile(_2m), Tile(_3m), Tile(_4p), Tile(_5p), Tile(_6p), Tile(_Z)};
  for (size_t i = 0; i < 7; i++) {
    hand.draw(Has + i);
  }
  Tile Seq[3] = {Tile(_7s), Tile(_8s), Tile(_9s)};
  Tile ConKong[4] = {Tile(_W), Tile(_W), Tile(_W), Tile(_W)};
  Tile new_tile = Tile(_Z);

  expose.add_new_meld(Meld(MeldType::Sequence, Seq, Seq + 1, Seq + 2));
  expose.add_new_meld(Meld(MeldType::ConcealedKong, ConKong, ConKong + 1, ConKong + 2, ConKong + 3));

  auto&& res = WinningDetector::detect(hand, expose, &new_tile);
  EXPECT_TRUE(res.has_result());
}