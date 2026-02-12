#ifndef TETRIS_CLI_H
#define TETRIS_CLI_H

#include <locale.h>
#include <ncurses.h>

#include "../../brick_game/tetris/tetris_lib.h"

/**
 * @brief Отрисовывает игровое поле в указанном окне.
 *
 * @param game_win Указатель на окно ncurses для игрового поля.
 * @param val Текущие данные игры для отображения.
 */
void printfield(WINDOW *game_win, Game_info_t val);

/**
 * @brief Отрисовывает игровую информацию (счет, уровень и т.д.) в указанном
 * окне.
 *
 * @param info_win Указатель на окно ncurses для информации.
 * @param val Текущие данные игры для отображения.
 */
void printinfo(WINDOW *info_win, Game_info_t val);

/**
 * @brief Освобождает ресурсы, связанные с окнами и завершает режим ncurses.
 *
 * @param game_win Указатель на окно игрового поля.
 * @param info_win Указатель на окно с информацией.
 */
void clear_win(WINDOW *game_win, WINDOW *info_win);

/**
 * @brief Считывает и интерпретирует ввод пользователя, возвращая действие.
 *
 * @return Действие пользователя в виде User_action_t.
 */
User_action_t read_input();

#endif
