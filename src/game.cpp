#include "game.h"

#include <utility>
#include <cstdlib>

//std::uniform_int_distribution<int> row_dist(1, this->game_map_.size());
//std::uniform_int_distribution<int> col_dist(1, this->game_map_[0].size());


Game::Game(std::string file) : file_(file) {
  load_map(file_);

  std::random_device r_;
  dev_ = std::mt19937{ r_() };

  Node bd1;  // head
  bd1.coord_ = std::make_pair(this->game_map_.size() / 2, this->game_map_[0].size() / 2);
  bd1.dir_ = Direction::L;
  snake_.body_.push_back(bd1);
}

void Game::clr(const char& snake_char, const char& food_char) {
  for (auto& str : this->game_map_) {
    for (auto& let : str) {
      (let == snake_char || let == food_char) ? (let = '_') : (let = let);
    }
  }

  system("CLS");
  //std::cout << "\033[2J\033[1;1H";
}
 
void Game::update_food() {
  int max = 3;
  int curr = std::accumulate(food_.begin(), food_.end(), 0, [](const int & total, const Food& n) {
      if (n.active_)
          return total + 1;
      else
          return total;
      });
  if (curr < max) {
      int row, col;
      bool pob;
      do {
          pob = false;
          std::uniform_int_distribution<int> row_dist(1, this->game_map_.size() - 1);
          std::uniform_int_distribution<int> col_dist(1, this->game_map_[0].size() - 1);
          row = row_dist(dev_);
          col = col_dist(dev_);
          if (this->game_map_[row][col] != '_') pob = true;
      } while (pob);

      Food food;
      food.active_ = true;
      food.coord_ = std::make_pair(row, col);
      food.growth_ = 1;
      food_.push_back(food);
  }
}

void Game::update_board(const char& input) {
  clr();

  for (const auto& body : snake_.body_) {
    set_pt(body.coord_.first, body.coord_.second);
  }

  for (const auto& food : food_) {
      if (food.active_) set_pt(food.coord_.first, food.coord_.second, 'X');
  }

  update_food();

  print_board();
}

void Game::add_body() { 
  this->snake_++; 
}

void Game::tnode_check(Node& body) {
    for (const auto& tnode : tnodes_) {
        if (body.coord_ == tnode.coord_) {
            body.dir_ = tnode.dir_;

            // TOOD: clear tnodes_
        }
    }
}

bool Game::advance_snake() {
  bool game = true;
  // if head can move, body can
  if (good_move()) {
    std::for_each(this->snake_.body_.begin(), this->snake_.body_.end(),
                  [](Node& n) { n++; });
  } else {
      std::cout << "YOU LOST!\n";
      game = false;
  }

  // check for body hitting turn nodes
  for (auto& body : this->snake_.body_) {
      tnode_check(body);
  }

  // check for body hitting food
  Node* head_ptr = &this->snake_.body_[0];
  for (size_t i = 0; i < food_.size(); i++) {
      if (head_ptr->coord_ == food_[i].coord_ && food_[i].active_) {
          snake_++;
          food_[i].active_ = false;
    }
  }
  return game;
}

bool Game::good_move() {
  bool val = true;

  const auto& head = this->snake_.body_[0];
  const int& row_h = head.coord_.first;
  const int& col_h = head.coord_.second;

  switch (head.dir_) {
    case Direction::U:
      if ((row_h - 1 < 1) || this->game_map_[row_h - 1][col_h] == 'O') {
        val = false;
      }
      break;
    case Direction::D:
      if ((row_h + 1 > game_map_.size() - 2) || this->game_map_[row_h + 1][col_h] == 'O') {
        val = false;
      }
      break;
    case Direction::L:
      if ((col_h - 1 < 1) || this->game_map_[row_h][col_h - 1] == 'O') {
        val = false;
      }
      break;
    case Direction::R:
      if (((col_h + 1) > (game_map_[0].size() - 2)) || this->game_map_[row_h][col_h + 1] == 'O') {
        val = false;
      }
      break;
    default:
      break;
  }

  return val;
}

void Game::load_map(const std::string& file_name) {
  std::ifstream input(file_name);
  if (!input.good()) {
    throw std::invalid_argument(file_name + ": invalid");
  }
  std::string line;
  while (std::getline(input, line)) {
    game_map_.push_back(line);
  }
}

void Game::print_board() {
  for (const auto& line : this->game_map_) {
    for (const auto& character : line) {
      std::cout << std::setw(2) << character;
    }
    std::cout << std::endl;
  }
}

void Game::turn(Direction new_dir) {
    Direction head_dir = snake_.body_[0].dir_;

    if (new_dir == Direction::U && head_dir == Direction::D) return;
    else if (new_dir == Direction::D && head_dir == Direction::U) return;
    else if (new_dir == Direction::R && head_dir == Direction::L) return;
    else if (new_dir == Direction::L && head_dir == Direction::R) return;

  snake_.body_[0].dir_ = new_dir;

  if (snake_.body_.size() > 1) {
      Node new_node;
      new_node.coord_ = snake_.body_[0].coord_;
      new_node.dir_ = new_dir;
      tnodes_.push_back(new_node);
  }
}
