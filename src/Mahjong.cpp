#include <memory>

#include "MajManager.hpp"

int main() {
  std::unique_ptr<MajManager> manager = std::make_unique<MajManager>();
  manager->begin();
  return 0;
}