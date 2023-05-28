#pragma once

#include <vector>

namespace mahjong {

class Expose;
class Hand;
class MeldInId;
struct WinningResult;

class Dora {
 public:
  static void TryMatch(const Hand& hand,
                       const Expose& expose,
                       bool Riichi,
                       WinningResult& result);
};
};  // namespace mahjong