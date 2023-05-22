#include "MajManager.hpp"

#include <memory>

int main() {
  auto&& maj_manager = mahjong::MajManager::GetMajManager();
  maj_manager->Begin();
  return 0;
}