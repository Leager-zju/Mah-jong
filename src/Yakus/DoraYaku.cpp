#include "DoraYaku.hpp"

#include "Expose.hpp"
#include "GlobalTileManager.hpp"
#include "Hand.hpp"
#include "TwoPointYaku.hpp"
#include "YakuMatcher.hpp"

namespace MAHJONG {
void Dora::try_match(const Hand& hand, const Expose& expose, bool Riichi,
                     MatchResult& result) {
  const std::unique_ptr<GlobalTileManager>& tile_manager =
      GlobalTileManager::get_tile_manager();
  for (auto&& tile : hand.get_hands()) {
    tile_id id = tile->to_id();
    if (tile_manager->is_dora(id)) {
      result.dora_++;
    }
    if (Riichi && tile_manager->is_inner_dora(id)) {
      result.inner_dora_++;
    }
  }
  for (auto&& meld : expose.get_exposes()) {
    for (auto tile : meld.meld_) {
      tile_id id = tile->to_id();
      if (tile_manager->is_dora(id)) {
        result.dora_++;
      }
      if (Riichi && tile_manager->is_inner_dora(id)) {
        result.inner_dora_++;
      }
    }
  }
}
};  // namespace MAHJONG