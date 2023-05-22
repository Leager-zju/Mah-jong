#pragma once

#include <vector>

#include "GlobalTileManager.hpp"

namespace MAHJONG {

class Expose;
class Hand;
class MeldInId;

class Dora {
 public:
  static void try_match(const Hand& hand, const Expose& expose, bool Riichi,
                        MatchResult& result);
};
};  // namespace MAHJONG