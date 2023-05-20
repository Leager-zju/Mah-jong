#include <cstdint>
#include "Expose.hpp"
#include "Hand.hpp"
#include "Rules/RuleMatcher.hpp"
#include "Rules/WhiteDragonRule.hpp"
#include "WinningDetector.hpp"
// #include "RuleMatcher.hpp"
#include "gtest/gtest.h"


TEST(Rule, WhiteDragonRule) {
  Hand hand;
  Expose expose;

  Tile has[13] = {
      Tile(_1m),   Tile(_2m), Tile(_3m), Tile(_4m), Tile(_5m),
      Tile(_6m), Tile(_7m), Tile(_8m), Tile(_9m), Tile(_B), Tile(_B), Tile(_B), Tile(_F)
  };
  for (size_t i = 0; i < 13; i++) {
    hand.draw(has + i);
  }
  Tile new_tile = Tile(_F);

  auto&& res = WinningDetector::detect(hand, expose, &new_tile);
  EXPECT_TRUE(res.has_result());

  uint16_t points = 0;
  EXPECT_TRUE(WhiteDragonRule::match(res, points));
}