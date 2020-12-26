#include <Windows.h>
#include <conio.h>

#include "game.h"

int main() {
  Game game("map.txt");

  while (true) {
    if (_kbhit()) {
      char a = _getch();
      if (a == 'w') {
        game.turn(Direction::U);
      } else if (a == 'a') {
        game.turn(Direction::L);
      } else if (a == 's') {
        game.turn(Direction::D);
        std::cout << std::endl;
      } else if (a == 'd') {
        game.turn(Direction::R);
      }
    } else {
      game.update_board('d');
      if (!game.advance_snake()) break;
      Sleep(100);
    }
  }

  return 0;
}
