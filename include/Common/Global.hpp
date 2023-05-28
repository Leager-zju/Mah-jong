#pragma once

#include <cstdint>
#include <functional>
#include <string>

namespace mahjong::global {
const char* GetColor(uint16_t player_index);
const char* GetWind(uint16_t player_index, uint16_t banker_index);
const char* GetRelativePosition(uint16_t other_index, uint16_t my_index);
const char* ColorOff();
std::string GetInput(
    const char* header,
    const std::function<bool(const std::string&)>& check_input);
}  // namespace mahjong::global
