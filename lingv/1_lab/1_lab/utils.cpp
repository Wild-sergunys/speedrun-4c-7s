#include "utils.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cctype>
#include <windows.h>

int get_int() {
  std::string input;
  int res = 0;

  for (;;) {
    std::getline(std::cin, input);

    if (input.empty()) {
      std::cout << "Ошибка: строка пуста. Попробуйте снова: ";
      continue;
    }

    try {
      size_t pos = 0;
      res = std::stoi(input, &pos);

      if (pos == input.length())
        return res;
      else
        std::cout << "Ошибка: введены лишние символы. Попробуйте снова: ";
    }
    catch (const std::invalid_argument&) {
      std::cout << "Ошибка: это не целое число. Попробуйте снова: ";
    }
    catch (const std::out_of_range&) {
      std::cout << "Ошибка: число слишком большое. Попробуйте снова: ";
    }
  }
}

bool get_yes_no() {
  std::string input;

  for (;;) {
    std::cout << " (y/n): ";
    std::getline(std::cin, input);

    if (input.length() == 1) {
      char ch = input[0];
      if (ch == 'y' || ch == 'Y') return true;
      if (ch == 'n' || ch == 'N') return false;
    }

    std::cout << "Ошибка: введите y или n.\n";
  }
}

std::string get_file_path_txt() {
  namespace fs = std::filesystem;

  std::string res;

  for (;;) {
    std::cout << "Введите путь к файлу: ";
    std::getline(std::cin, res);

    size_t start = res.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
      std::cout << "Ошибка: путь не может быть пустым.\n";
      continue;
    }
    res = res.substr(start);

    fs::path file_path(res);

    if (!file_path.is_absolute())
      file_path = fs::current_path() / file_path;

    std::string ext = file_path.extension().string();
    for (char& c : ext)
      c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    if (ext != ".txt") {
      std::cout << "Ошибка: файл должен иметь расширение .txt.\n";
      continue;
    }

    if (!fs::exists(file_path)) {
      std::cout << "Ошибка: файл не найден.\n";
      continue;
    }

    if (!fs::is_regular_file(file_path)) {
      std::cout << "Ошибка: это не файл, а папка.\n";
      continue;
    }

    std::ifstream test(file_path);
    if (!test.is_open()) {
      std::cout << "Ошибка: файл не открывается для чтения.\n";
      continue;
    }
    test.close();

    return file_path.string();
  }
}

bool save_res_to_file(const std::string& data) {
  namespace fs = std::filesystem;

  const std::string INVALID_CHARS = "\\/:*?\"<>|";
  const std::vector<std::string> RESERVED_NAMES = {
      "CON", "PRN", "AUX", "NUL",
      "COM1", "COM2", "COM3", "COM4", "COM5",
      "COM6", "COM7", "COM8", "COM9",
      "LPT1", "LPT2", "LPT3", "LPT4", "LPT5",
      "LPT6", "LPT7", "LPT8", "LPT9"
  };

  std::string path;

  for (;;) {
    std::cout << "Введите путь для сохранения (только имя файла или полный путь): ";
    std::getline(std::cin, path);

    size_t start = path.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
      std::cout << "Ошибка: путь не может быть пустым.\n";
      continue;
    }
    path = path.substr(start);

    fs::path file_path(path);

    if (!file_path.is_absolute())
      file_path = fs::current_path() / file_path;

    std::string filename = file_path.filename().string();

    bool has_invalid = false;
    for (char c : filename) {
      if (INVALID_CHARS.find(c) != std::string::npos) {
        has_invalid = true;
        break;
      }
    }

    if (has_invalid) {
      std::cout << "Ошибка: имя файла содержит запрещённые символы (\\/:*?\"<>|).\n";
      continue;
    }

    std::string name = filename;
    size_t dot_pos = name.find('.');
    if (dot_pos != std::string::npos)
      name = name.substr(0, dot_pos);
    for (char& c : name)
      c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

    bool is_reserved = false;
    for (const std::string& res : RESERVED_NAMES) {
      if (name == res) {
        is_reserved = true;
        break;
      }
    }

    if (is_reserved) {
      std::cout << "Ошибка: имя файла зарезервировано для Windows.\n";
      continue;
    }

    if (fs::exists(file_path)) {
      std::cout << "Файл уже существует. Перезаписать его?";
      if (!get_yes_no()) {
        std::cout << "Сохранение отменено.\n";
        continue;
      }
    }

    std::ofstream test(file_path);
    if (!test.is_open()) {
      std::cout << "Ошибка: невозможно создать файл. Проверьте права доступа.\n";
      continue;
    }
    test.close();
    fs::remove(file_path);

    std::ofstream file(file_path);
    if (!file.is_open()) {
      std::cout << "Ошибка: не удалось открыть файл для записи.\n";
      return false;
    }

    file << data;
    file.close();

    std::cout << "Файл сохранён: " << file_path.string() << std::endl;
    return true;
  }
}

std::string utf8_to_cp1251(const std::string& utf8_str) {
#ifdef _WIN32
  if (utf8_str.empty()) return "";

  int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
  if (wlen == 0) return utf8_str;

  std::wstring wstr(wlen, L'\0');
  MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, wstr.data(), wlen);

  int len = WideCharToMultiByte(1251, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
  if (len == 0) return utf8_str;

  std::string res(len, '\0');
  WideCharToMultiByte(1251, 0, wstr.c_str(), -1, res.data(), len, nullptr, nullptr);

  if (!res.empty() && res.back() == '\0') res.pop_back();
  return res;
#else
  return utf8_str;
#endif
}