#include "Expose.hpp"

#include <iostream>

void Expose::add_triplet(std::vector<pTile>&& triplet) { in_expose_.emplace_back(std::move(triplet)); }
void Expose::add_expose_kong(std::vector<pTile>&& expose_kong) { in_expose_.emplace_back(std::move(expose_kong)); }
void Expose::add_concealed_kong(std::vector<pTile>&& concealed_kong) { in_expose_.emplace_back(std::move(concealed_kong)); }
void Expose::add_sequence(std::vector<pTile>&& sequence) { in_expose_.emplace_back(std::move(sequence)); }

void Expose::show() {
  std::cout << "Expose: ";
  for (int i = 0; i < in_expose_.size(); i++) {
    for (int j = 0; j < in_expose_[i].size(); i++) {
      std::cout << in_expose_[i][j]->to_string();
    }
    std::cout << " ";
  }
  std::cout << '\n';
}