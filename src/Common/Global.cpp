#include "Global.hpp"

#include <cstdint>

namespace mahjong {
namespace global {
  // COLOR
  const char* red       = "\033[31m";
  const char* green     = "\033[32m";
  const char* yellow    = "\033[33m";
  const char* blue      = "\033[34m";
  const char* color_off = "\033[0m";

  // WIND
  const char* east  = "东";
  const char* south = "南";
  const char* west  = "西";
  const char* north = "北";

  const char* GetColor(uint16_t player_index) {
    switch (player_index) {
      case 0:
        return red;
      case 1:
        return green;
      case 2:
        return yellow;
      case 3:
        return blue;
      default:
        return "";
    }
  }

  const char* GetWind(uint16_t player_index, uint16_t banker_index) {
    uint16_t diff = (player_index - banker_index + 4) % 4;
    switch (diff) {
      case 0:
        return east;
      case 1:
        return south;
      case 2:
        return west;
      case 3:
        return north;
      default:
        return "";
    }
  }

  const char* GetRelativePosition(uint16_t other_index, uint16_t my_index) {
    uint16_t diff = (other_index - my_index + 4) % 4;
    switch (diff) {
      case 1:
        return "下家";
      case 2:
        return "对家";
      case 3:
        return "上家";
      default:
        return "";
    }
  }
  const char* ColorOff() {
    return color_off;
  }
};  // namespace global
};  // namespace mahjong