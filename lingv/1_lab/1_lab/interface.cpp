#include "interface.h"
#include "algorithms.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

std::string read_file(const std::string& path) {
  std::ifstream file(path);
  std::string res;
  std::string line;

  while (std::getline(file, line))
    res += utf8_to_cp1251(line) + "\n";

  return res;
}

std::string input_text() {
  std::cout << "Введите текст (для завершения ввода введите пустую строку):\n";
  std::cout << "Для отмены введите Ctrl+Z\n";
  std::string res;
  std::string line;

  for (;;) {
    if (!std::getline(std::cin, line)) {
      std::cin.clear();
      std::cout << "\nВвод отменен. Возврат в главное меню.\n";
      return res;
    }

    if (line.empty()) break;
    res += line + "\n";
  }

  return res;
}

void process_text(const std::string& text) {
  if (text.empty()) {
    std::cout << "Ошибка: текст пуст.\n";
    return;
  }

  std::cout << "Введите символ для поиска: ";
  char ch;
  std::cin >> ch;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  std::string res = find_words_with_char(text, ch);

  if (res.empty()) {
    std::cout << "Слова, содержащие символ '" << ch << "', не найдены.\n";
    return;
  }

  std::cout << "\nСлова, содержащие символ '" << ch << "':\n";
  std::cout << res << "\n";

  std::cout << "\nСохранить результат в файл?";
  if (get_yes_no())
    if (!save_res_to_file(res))
      std::cout << "Сохранение отменено или произошла ошибка.\n";
}

void run_tests() {
  struct TestCase {
    std::string text;
    char ch;
    std::string expected;
  };

  std::vector<TestCase> tests;
  tests.push_back({ "hello world", 'l', "hello world" });
  tests.push_back({ "йоу есс", 'x', "" });
  tests.push_back({ "test", 'x', "" });
  tests.push_back({ "", 'a', "" });
  tests.push_back({ "hello", 'h', "hello" });
  tests.push_back({ "one two three", 't', "two three" });
  tests.push_back({ "cat dog", 'z', "" });

  bool all_passed = true;

  for (const auto& test : tests) {
    std::string res = find_words_with_char(test.text, test.ch);
    if (res != test.expected) {
      std::cout << "FAIL: Текст: \"" << test.text << "\", "
        << "Символ: '" << test.ch << "', "
        << "Ожидалось: \"" << test.expected << "\", "
        << "Получено: \"" << res << "\"\n";
      all_passed = false;
    }
  }

  if (all_passed)
    std::cout << "Тестирование прошло успешно.\n";
}