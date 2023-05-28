#include "RiichiDetector.hpp"

#include "Common.hpp"
#include "Expose.hpp"
#include "Hand.hpp"
#include "Tiles.hpp"

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace mahjong {

void WaitingResult::Append(std::set<TileId>&& waiting_for) {
  for (auto&& tile_id : waiting_for) {
    waiting_for_.insert(tile_id);
  }
}

std::vector<WaitingResult> riichi_detector::CheckRiichi(const Hand& hand,
                                                        const Expose& expose) {
  // Make Sure Menzenchin
  for (auto&& meld : expose.GetExposes()) {
    if (meld.GetMeldType() != MeldType::ConcealedKong) {
      return {};
    }
  }

  // make table
  std::unordered_map<TileId, size_t> table;
  std::vector<WaitingResult> results;
  for (auto&& tile : hand.GetHands()) {
    TileId id = tile->GetId();
    if (table.count(id)) {
      table[id]++;
    } else {
      table.emplace(id, 1);
    }
  }

  if (CheckThirteenOrphans(table, results)) {
    return results;
  }
  if (CheckSevenPairs(table, results)) {
    return results;
  }

  std::unordered_map<TileId, size_t> first_index{};
  Reduce(table, false, results, first_index);

  std::sort(results.begin(),
            results.end(),
            [](const WaitingResult& lhs, const WaitingResult& rhs) -> bool {
              return lhs.GetDiscard() < rhs.GetDiscard();
            });
  return results;
}

bool riichi_detector::CheckThirteenOrphans(
    std::unordered_map<TileId, size_t>& table,
    std::vector<WaitingResult>& results) {
  if (table.size() >= 13) {
    TileId discard = NAT;
    std::set<TileId> orphans{_1m, _9m, _1p, _9p, _1s, _9s, E, S, W, N, Z, B, F};
    uint16_t count = 0;
    for (auto&& pair : table) {
      if (Tile::IsTerminal(pair.first)) {
        if (pair.second == 2 && discard == NAT) {
          discard = pair.first;
        }
        count++;
      } else if (discard == NAT || Tile::IsTerminal(discard)) {
        discard = pair.first;
      } else {
        break;
      }
    }
    if (count == 13) {
      results.emplace_back(discard, orphans);
      if (Tile::IsTerminal(discard)) {
        for (auto&& id : orphans) {
          if (id != discard) {
            results.emplace_back(WaitingResult(id, {discard}));
          }
        }
      }
    } else if (count == 12) {
      for (auto&& id : orphans) {
        if (table.count(id) == 0) {
          results.emplace_back(WaitingResult(discard, {id}));
          break;
        }
      }
    }
  }
  return !results.empty();
}

bool riichi_detector::CheckSevenPairs(std::unordered_map<TileId, size_t>& table,
                                      std::vector<WaitingResult>& results) {
  if (table.size() == 7) {
    for (auto&& pair : table) {
      if (pair.second != 2) {
        return false;
      }
    }
    for (auto&& pair : table) {
      results.emplace_back(WaitingResult(pair.first, {pair.first}));
    }
  } else if (table.size() == 8) {
    TileId first  = NAT;
    TileId second = NAT;
    for (auto&& pair : table) {
      if (pair.second == 1) {
        if (first == NAT) {
          first = pair.first;
        } else if (second == NAT) {
          second = pair.first;
        } else {
          return false;
        }
      } else if (pair.second > 2) {
        return false;
      }
    }

    results.emplace_back(WaitingResult(first, {second}));
    results.emplace_back(WaitingResult(second, {first}));
  }

  return !results.empty();
}

void riichi_detector::Reduce(std::unordered_map<TileId, size_t>& table,
                             bool find_eyes,
                             std::vector<WaitingResult>& results,
                             std::unordered_map<TileId, size_t>& first_index) {
  uint16_t count = 0;
  for (auto&& iter = table.begin(); iter != table.end(); iter++) {
    count += iter->second;
    if (iter->second >= 3) {
      iter->second -= 3;
      Reduce(table, find_eyes, results, first_index);
      iter->second += 3;
    }

    if (iter->second >= 2 && !find_eyes) {
      iter->second -= 2;
      Reduce(table, true, results, first_index);
      iter->second += 2;
    }

    if (iter->second >= 1) {
      auto plus1 = static_cast<TileId>(iter->first + 1);
      auto plus2 = static_cast<TileId>(iter->first + 2);
      if (table.count(plus1) == 0 || table[plus1] == 0
          || table.count(plus2) == 0 || table[plus2] == 0) {
        continue;
      }
      iter->second--;
      table[plus1]--;
      table[plus2]--;
      Reduce(table, find_eyes, results, first_index);
      iter->second++;
      table[plus1]++;
      table[plus2]++;
    }
  }

  if (count > 3 || count < 2) {
    return;
  }
  if (!find_eyes) {
    return;
  }

  // 剩下不成型的 2~3 张还要打一张出去才能听
  std::vector<TileId> remains;
  for (auto&& pair : table) {
    for (auto i = 0; i < pair.second; i++) {
      remains.push_back(pair.first);
    }
  }

  if (remains.size() == 2) {
    if (first_index.count(remains[0])) {
      results[first_index[remains[0]]].Append({remains[1]});
    } else {
      first_index[remains[0]] = results.size();
      results.push_back(WaitingResult(remains[0], {remains[1]}));
    }

    if (first_index.count(remains[1])) {
      results[first_index[remains[1]]].Append({remains[0]});
    } else {
      first_index[remains[1]] = results.size();
      results.push_back(WaitingResult(remains[1], {remains[0]}));
    }
    return;
  }

  if (auto&& res = Tile::FindMeldComplement(remains[1], remains[2]);
      !res.empty()) {
    if (first_index.count(remains[0])) {
      results[first_index[remains[0]]].Append(std::move(res));
    } else {
      first_index[remains[0]] = results.size();
      results.emplace_back(remains[0], res);
    }
  }

  if (auto&& res = Tile::FindMeldComplement(remains[0], remains[2]);
      !res.empty()) {
    if (first_index.count(remains[1])) {
      results[first_index[remains[1]]].Append(std::move(res));
    } else {
      first_index[remains[1]] = results.size();
      results.emplace_back(remains[1], res);
    }
  }

  if (auto&& res = Tile::FindMeldComplement(remains[0], remains[1]);
      !res.empty()) {
    if (first_index.count(remains[2])) {
      results[first_index[remains[2]]].Append(std::move(res));
    } else {
      first_index[remains[2]] = results.size();
      results.emplace_back(remains[2], res);
    }
  }
}
}  // namespace mahjong