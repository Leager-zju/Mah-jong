#include "DoraYaku.hpp"

#include "Expose.hpp"
#include "GlobalTileManager.hpp"
#include "Hand.hpp"
#include "TwoPointYaku.hpp"
#include "YakusMatcher.hpp"

namespace mahjong {
void Dora::TryMatch(const Hand& hand,
                    const Expose& expose,
                    bool Riichi,
                    MatchResult& result) {
  const std::unique_ptr<GlobalTileManager>& tile_manager
      = GlobalTileManager::GetTileManager();
  for (auto&& tile : hand.GetHands()) {
    TileId id = tile->ToId();
    if (tile_manager->IsDora(id)) {
      result.AddDora(1);
    }
    if (Riichi && tile_manager->IsInnerDora(id)) {
      result.AddInnerDora(1);
    }
  }
  for (auto&& meld : expose.GetExposes()) {
    for (auto tile : meld.GetMeld()) {
      TileId id = tile->ToId();
      if (tile_manager->IsDora(id)) {
        result.AddDora(1);
      }
      if (Riichi && tile_manager->IsInnerDora(id)) {
        result.AddInnerDora(1);
      }
    }
  }
}
};  // namespace mahjong