#pragma once

#include <cstdint>
#include <vector>
#include "Meld.hpp"
#include "WinningDetector.hpp"

class WhiteDragonRule {
  public:
  static bool match(const DetectResult& detector_result, uint16_t& points) {
    const std::vector<MeldInId>& melds = detector_result.meld_in_id_;
    for (auto&& meld : melds) {
      if (meld.meld_type_ == MeldType::Triplet && meld.tile_ids_[0] == _B) {
        return true;
      }
    }
    return false;
  }
};