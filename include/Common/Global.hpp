#pragma once

#include <cstdint>

namespace mahjong::global {
const char* GetColor(uint16_t player_index);
const char* GetWind(uint16_t player_index, uint16_t banker_index);
const char* GetRelativePosition(uint16_t other_index, uint16_t my_index);
const char* ColorOff();
}  // namespace mahjong::global
