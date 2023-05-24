#include "Expose.hpp"

#include "Yakus/Meld.hpp"

namespace mahjong {
void Expose::Show() const {
  for (auto&& meld : in_expose_) {
    meld.Show();
  }
  if (in_expose_.empty()) {
    std::cout << '\n';
  }
}

void Expose::AddNewMeld(const Meld& meld) {
  in_expose_.emplace_back(meld);
}
};  // namespace mahjong