#include "tetris_cli.h"

int main() {
  setlocale(LC_ALL, "");
  srand(time(NULL));
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);

  WINDOW *game_win = newwin(22, 24, 0, 0);
  WINDOW *info_win = newwin(22, 24, 0, 24);

  User_action_t action;
  while ((action = read_input()) != Quit) {
    user_input(action);
    Game_info_t tetris = get_game_info();
    printfield(game_win, tetris);
    printinfo(info_win, tetris);
    napms(50);
  }

  clear_win(game_win, info_win);
  return 0;
}

void printfield(WINDOW *game_win, Game_info_t tetris) {
  werase(game_win);
  box(game_win, 0, 0);

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (tetris.field[i][j]) {
        mvwprintw(game_win, i + 1, (j + 1) * 2, "[]");
      }
    }
  }
  wrefresh(game_win);
}

void printinfo(WINDOW *info_win, Game_info_t tetris) {
  werase(info_win);

  box(info_win, 0, 0);
  for (int i = 0; i < 4 && tetris.status != Game_over; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetris.next_figure[i][j]) {
        mvwprintw(info_win, 9 + i, 7 + j * 2, "[]");
      }
    }
  }
  mvwprintw(info_win, 1, 1, "HI-Score: %d", tetris.hi_score);
  mvwprintw(info_win, 3, 1, "Score: %d", tetris.score);
  mvwprintw(info_win, 5, 1, "lvl: %d", tetris.lvl);
  mvwprintw(info_win, 7, 1, "Speed: %d", tetris.lvl + 1);
  if (tetris.pause) {
    mvwprintw(info_win, 13, 9, "PAUSE");
  }
  if (tetris.status == Start_init || tetris.status == Game_over) {
    mvwprintw(info_win, 13, 3, "PRESS \'s\' TO START");
  }
  if (tetris.status == Game_over) {
    mvwprintw(info_win, 15, 6, "GAME OVER");
  }
  mvwprintw(info_win, 17, 6, "Controls:");
  mvwprintw(info_win, 18, 1, "← → - Move; ↑ - Rotate");
  mvwprintw(info_win, 19, 1, "↓ - Fast drop");
  mvwprintw(info_win, 20, 1, "p - Pause; q - Quit");
  wrefresh(info_win);
}

void clear_win(WINDOW *game_win, WINDOW *info_win) {
  delwin(game_win);
  delwin(info_win);
  endwin();
}

User_action_t read_input() {
  int ch = getch();
  flushinp();
  User_action_t action = Err;
  if (ch == KEY_DOWN) {
    action = Down;
  } else if (ch == KEY_LEFT) {
    action = Left;
  } else if (ch == KEY_RIGHT) {
    action = Right;
  } else if (ch == KEY_UP) {
    action = Action;
  } else if (ch == 'q') {
    action = Quit;
  } else if (ch == 'p') {
    action = Pause;
  } else if (ch == 's') {
    action = Start;
  }
  return action;
}