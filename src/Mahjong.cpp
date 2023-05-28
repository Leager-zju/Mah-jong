#include "MajManager.hpp"

#include <cstdio>
#include <memory>

int main() {
  system("chcp 65001");
  std::cout << "PRESS ANY KEY TO BEGIN...\n";
  getchar();

  auto&& maj_manager = mahjong::MajManager::GetMajManager();
  maj_manager->Begin();

  std::cout << "PRESS ANY KEY TO END...\n";
  getchar();
  return 0;
}