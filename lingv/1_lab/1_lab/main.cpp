#include "interface.h"
#include "utils.h"
#include <iostream>
#include <windows.h>

#define GREETING \
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" \
    "@                                   @\n"  \
    "@      Лабораторная работа № 1      @\n"   \
    "@             Вариант № 5           @\n"    \
    "@   Выполнили студеты группы 434:   @\n"     \
    "@  Щербаков С.И. && Журавлева А.В.  @\n"      \
    "@                                   @\n"       \
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n"

#define TASK          \
    "Текст задания:\n" \
    "5. Задан текст. Распечатать все слова, содержащие заданный символ.\n\n"

#define MENU                      \
    "\nВыберите:\n"                \
    " [1] - Использовать текст из файла (.txt);\n" \
    " [2] - Ввести текст вручную;\n"               \
    " [3] - Запустить тесты;\n"                    \
    " [4] - Завершить программу.\n"

enum menu_choice {
  FILE_PARSING = 1,
  MANUAL_INPUT = 2,
  RUN_TESTS = 3,
  EXIT = 4
};

int main() {
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  setlocale(LC_ALL, "Russian");

  std::cout << GREETING << TASK;

  for (;;) {
    std::cout << MENU;
    int choice = get_int();

    switch (choice) {
    case FILE_PARSING: {
      std::string file_path = get_file_path_txt();
      std::string text = read_file(file_path);

      if (text.empty()) {
        std::cout << "Ошибка: файл пуст.\n";
        break;
      }

      std::cout << "\n@@@ Содержимое файла @@@\n";
      std::cout << text << "\n";

      process_text(text);
      break;
    }

    case MANUAL_INPUT: {
      std::string text = input_text();
      process_text(text);
      break;
    }

    case RUN_TESTS:
      run_tests();
      break;

    case EXIT:
      std::cout << "Программа завершена.\n";
      return 0;

    default:
      std::cout << "Ошибка: выберите 1, 2, 3 или 4.\n";
      break;
    }
  }
}