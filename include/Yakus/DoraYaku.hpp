#pragma once

#include <vector>

namespace mahjong {

class Expose;
class Hand;
class MeldInId;
struct MatchResult;

class Dora {
 public:
  static void TryMatch(const Hand& hand,
                       const Expose& expose,
                       bool Riichi,
                       MatchResult& result);
};
};  // namespace mahjong