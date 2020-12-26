#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <numeric>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <random>

enum Direction { U, D, L, R, N };

struct Node {
  std::pair<int, int> coord_;
  Direction dir_;

  Node& operator++(int) {
    switch (this->dir_) {
      case Direction::U:
        this->coord_.first -= 1;
        break;
      case Direction::D:
        this->coord_.first += 1;
        break;
      case Direction::L:
        this->coord_.second -= 1;
        break;
      case Direction::R:
        this->coord_.second += 1;
        break;
      default:
        break;
    }

    return *this;
  }
};

struct Snake {
  std::vector<Node> body_;

  Snake& operator++(int) {
    auto* tail = &(this->body_.back());
    Node new_tail;
    new_tail = *tail;

    switch (tail->dir_) {
      case Direction::U:
        new_tail.dir_ = Direction::U;
        new_tail.coord_.first += 1;
        break;
      case Direction::D:
        new_tail.dir_ = Direction::D;
        new_tail.coord_.first -= 1;
        break;
      case Direction::L:
        new_tail.dir_ = Direction::L;
        new_tail.coord_.second += 1;
        break;
      case Direction::R:
        new_tail.dir_ = Direction::R;
        new_tail.coord_.second -= 1;
        break;
      default:
        break;
    }

    this->body_.push_back(new_tail);
    return *this;
  }
};

struct Food {
    std::pair<int, int> coord_;
    int growth_;
    bool active_;
};

class Game {
 private:
  Snake snake_;
  std::string file_;
  std::vector<std::string> game_map_;
  std::vector<Food> food_;
  std::vector<Node> tnodes_;
  std::mt19937 dev_;

  //int max_food_;

  void load_map(const std::string& file_name);
  void set_pt(const int& row, const int& col, const char& del = 'O') {
    game_map_[row][col] = del;
  }
  void clr(const char& snake_char = 'O', const char& food_char = '*');
  void tnode_check(Node& body);
  void update_food();
  void add_body();
  void init_food();
  bool good_move();

 public:
  Game(std::string file);
  ~Game() { }


  bool advance_snake();
  void update_board(const char& input);
  void print_board();

  void turn(Direction new_dir);
};
#endif  // GAME_H
