#include "Expose.hpp"

#include "Yakus/Meld.hpp"

namespace MAHJONG {
void Expose::show() const {
  std::cout << "Expose:";
  for (auto&& meld : in_expose_) {
    meld.show();
  }
  if (in_expose_.empty()) {
    std::cout << '\n';
  }
}

void Expose::add_new_meld(const Meld& meld) { in_expose_.emplace_back(meld); }
};  // namespace MAHJONG