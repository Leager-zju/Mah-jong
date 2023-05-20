#include <memory>

#include "MajManager.hpp"

int main() {
  srand(static_cast<unsigned int>(time(nullptr)));

  std::unique_ptr<MajManager> manager = std::make_unique<MajManager>();
  manager->begin();
  return 0;
}