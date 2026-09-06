#include "utils.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cctype>
#include <sstream>
#include <limits>
#include <chrono>
#include <iomanip>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

constexpr int DATE_LENGTH = 10;
constexpr int YEAR_END = 4;
constexpr int MONTH_END = 7;
constexpr int MIN_YEAR = 1825;  // год начала использования бензина

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

      if (pos == input.length()) return res;
      else std::cout << "Ошибка: введены лишние символы. Попробуйте снова: ";
    }
    catch (const std::invalid_argument&) {
      std::cout << "Ошибка: это не целое число. Попробуйте снова: ";
    }
    catch (const std::out_of_range&) {
      std::cout << "Ошибка: число слишком большое. Попробуйте снова: ";
    }
  }
}

double get_double() {
  std::string input;
  double res = 0.0;
  for (;;) {
    std::getline(std::cin, input);
    if (input.empty()) {
      std::cout << "Ошибка: строка пуста. Попробуйте снова: ";
      continue;
    }
    try {
      size_t pos = 0;
      res = std::stod(input, &pos);
      if (pos == input.length()) return res;
      else std::cout << "Ошибка: введены лишние символы. Попробуйте снова: ";
    }
    catch (...) {
      std::cout << "Ошибка: введите число. Попробуйте снова: ";
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
    res = trim(res);
    if (res.empty()) {
      std::cout << "Ошибка: путь не может быть пустым.\n";
      continue;
    }
    fs::path file_path(res);
    if (!file_path.is_absolute())
      file_path = fs::current_path() / file_path;
    std::string ext = file_path.extension().string();
    for (char& c : ext) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c))); // uc, т.к. tolower ожидает от 0..255
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

std::string get_date_from_user() {
  std::string date;
  for (;;) {
    std::getline(std::cin, date);
    date = trim(date);
    if (is_valid_date(date)) return date;
    else std::cout << "Ошибка: неверный формат даты. Используйте YYYY-MM-DD.\n";
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
    std::cout << "Введите путь для сохранения: ";
    std::getline(std::cin, path);
    path = trim(path);
    if (path.empty()) {
      std::cout << "Ошибка: путь не может быть пустым.\n";
      continue;
    }
    fs::path file_path(path);
    if (!file_path.is_absolute())
      file_path = fs::current_path() / file_path;

    std::string filename = file_path.filename().string();

    bool has_invalid = false;
    for (char c : filename)
      if (INVALID_CHARS.find(c) != std::string::npos) {
        has_invalid = true;
        break;
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
    for (const std::string& res : RESERVED_NAMES)
      if (name == res) {
        is_reserved = true;
        break;
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
    std::ofstream file(file_path);
    if (!file.is_open()) {
      std::cout << "Ошибка: не удалось создать файл.\n";
      continue;
    }
    file << data;
    file.close();
    std::cout << "Файл сохранён: " << file_path.string() << std::endl;
    return true;
  }
}

std::string trim(const std::string& s) {
  size_t start = s.find_first_not_of(" \t\n\r\f\v");
  if (start == std::string::npos) return "";
  size_t end = s.find_last_not_of(" \t\n\r\f\v");
  return s.substr(start, end - start + 1);
}

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter))
    tokens.push_back(trim(token));
  return tokens;
}

bool is_valid_date(const std::string& date) {
  if (date.length() != DATE_LENGTH) return false;
  if (date[YEAR_END] != '-' || date[MONTH_END] != '-') return false;

  for (char c : date)
    if (c != '-' && !std::isdigit(c)) return false;

  int y = std::stoi(date.substr(0, 4));
  int m = std::stoi(date.substr(5, 2));
  int d = std::stoi(date.substr(8, 2));

  return y >= MIN_YEAR &&
    m >= 1 && m <= 12 &&
    d >= 1 && d <= 31;
}

int days_between(const std::string& date1, const std::string& date2) {
  if (!is_valid_date(date1) || !is_valid_date(date2)) return 0;

  try {
    auto parse_date = [](const std::string& d) {
      int year = std::stoi(d.substr(0, 4));
      int month = std::stoi(d.substr(5, 2));
      int day = std::stoi(d.substr(8, 2));

      std::tm tm = {};
      tm.tm_year = year - 1900;
      tm.tm_mon = month - 1;
      tm.tm_mday = day;

      std::time_t time = std::mktime(&tm);
      return time;
      };

    std::time_t t1 = parse_date(date1);
    std::time_t t2 = parse_date(date2);

    if (t1 == -1 || t2 == -1) return 0;

    double diff_seconds = std::difftime(t2, t1);
    int days = static_cast<int>(diff_seconds / (60 * 60 * 24));
    return days > 0 ? days : 0;
  }
  catch (...) {
    return 0;
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