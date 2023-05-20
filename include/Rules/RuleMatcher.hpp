#pragma once

#include <cstdint>
#include <string>

#include "Rules/WhiteDragonRule.hpp"
#include "Tiles.hpp"
#include "WhiteDragonRule.hpp"
#include "WinningDetector.hpp"


class RuleMatcher {
 public:
  static std::string try_match(const DetectResult& detect_result, uint16_t& points) {
    std::string match_result;
    if (WhiteDragonRule::match(detect_result, points)) {
      match_result += "Honor White ---------- 1 point\n";
    }


    match_result += "Total Points: ";
    match_result.push_back(static_cast<char>(points));
    return match_result;
  }
};