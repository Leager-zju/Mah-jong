#pragma once

#include <cassert>
#include <cstdint>

enum class MeldType : uint16_t {
  Eyes,           // 雀头
  Triplet,        // 刻子
  ExposeKong,     // 明杠
  ConcealedKong,  // 暗杠
  Sequence,       // 顺子
};

class Meld {
 public:
  MeldType meld_type_;
  virtual void show() = 0;
};

class Eyes : public Meld {
 public:
  explicit Eyes(uint16_t id) {
    Meld::meld_type_ = MeldType::Eyes;
    tile_id_ = id;
  }
  virtual void show();

 private:
  uint16_t tile_id_;
};

class Triplet : public Meld {
 public:
  explicit Triplet(uint16_t id) {
    Meld::meld_type_ = MeldType::Eyes;
    tile_id_ = id;
  }
  virtual void show();

 private:
  uint16_t tile_id_;
};

class Sequence : public Meld {
 public:
  explicit Sequence(uint16_t id1, uint16_t id2, uint16_t id3) {
    assert(id2 == id1 + 1 && id3 == id2 + 1);
    Meld::meld_type_ = MeldType::Eyes;
    tile_id_[0] = id1;
    tile_id_[1] = id2;
    tile_id_[2] = id3;
  }
  virtual void show();

 private:
  uint16_t tile_id_[3];
};

class ExposeKong : public Meld {
 public:
  explicit ExposeKong(uint16_t id) {
    Meld::meld_type_ = MeldType::ExposeKong;
    tile_id_ = id;
  }
  virtual void show();

 private:
  uint16_t tile_id_;
};

class ConcealedKong : public Meld {
 public:
  explicit ConcealedKong(uint16_t id) {
    Meld::meld_type_ = MeldType::ConcealedKong;
    tile_id_ = id;
  }
  virtual void show();

 private:
  uint16_t tile_id_;
};