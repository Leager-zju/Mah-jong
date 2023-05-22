#include <memory>

#include "MajManager.hpp"

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  std::unique_ptr<MAHJONG::MajManager>& maj_manager =
      MAHJONG::MajManager::get_maj_manager();
  maj_manager->begin();
  return 0;
}