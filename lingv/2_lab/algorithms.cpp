#include "algorithms.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <cmath>
#include <locale>


void print_purchase(const FuelPurchase& p, const FuelPurchase* prev) {
  std::cout << std::fixed << std::setprecision(2); // fixed - обычный формат, не научный, 2 знака после запятой
  std::cout << "Дата: " << p.date
    << " | Марка: " << p.brand
    << " | Одометр: " << p.odometer
    << " | Цена/л: " << p.price_per_liter
    << " | Литров: " << p.liters
    << " | Стоимость: " << p.total_cost;

  if (prev) {
    double km = p.odometer - prev->odometer;
    double km_per_l = (p.liters > 0) ? km / p.liters : 0;
    double cost_per_km = (km > 0) ? p.total_cost / km : 0;
    double cost_per_gallon = p.price_per_liter * GALLON_TO_LITER;
    double days = days_between(prev->date, p.date);
    double cost_per_day = (days > 0) ? p.total_cost / days : 0;
    double gallons = p.liters / GALLON_TO_LITER;
    double days_per_gallon = (gallons > 0 && days > 0) ? days / gallons : 0;

    std::cout << " | Пробег: " << km
      << " | Км/л: " << km_per_l
      << " | Руб/км: " << cost_per_km
      << " | Руб/галлон: " << cost_per_gallon
      << " | Дней между: " << days
      << " | Руб/день: " << cost_per_day
      << " | Дней/галлон: " << days_per_gallon;
  }
  std::cout << "\n";
}

void print_statistics(const Statistics& s, const std::string& label) {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "\n---- " << label << " ----\n";
  std::cout << "Всего км: " << s.total_km << "\n";
  std::cout << "Всего литров: " << s.total_liters << "\n";
  std::cout << "Всего затрат: " << s.total_cost << " руб." << "\n";
  std::cout << "Всего дней: " << s.total_days << "\n";
  std::cout << "Количество заправок: " << s.count << "\n";
  if (s.count > 0) {
    std::cout << "Средний расход (км/л): " << s.avg_km_per_liter << "\n";
    std::cout << "Средняя стоимость км: " << s.avg_cost_per_km << " руб." << "\n";
    std::cout << "Средняя стоимость галлона: " << s.avg_cost_per_gallon << " руб." << "\n";
    if (s.total_days > 0 && s.count > 1) {
      std::cout << "Средняя стоимость дня: " << s.avg_cost_per_day << " руб." << "\n";
      std::cout << "Среднее дней на галлон: " << s.avg_days_per_gallon << "\n";
    }
  }
}

std::vector<FuelPurchase> parse_fuel_data(const std::vector<std::string>& lines, std::string& error) {
  std::vector<FuelPurchase> res;
  error = "";

  if (lines.empty()) {
    error = "Файл пуст";
    return res;
  }

  std::locale c_locale("C"); // т.к. общая локаль ru

  for (size_t line_num = 0; line_num < lines.size(); ++line_num) {
    const std::string& line = lines[line_num];
    if (line.empty()) continue;

    // Разбитие на поля 
    std::vector<std::string> tokens;
    std::string current_token;
    bool inside_quotes = false;
    std::stringstream ss(line);
    char ch;

    while (ss.get(ch)) {
      if (ch == ',' && !inside_quotes) {
        tokens.push_back(trim(current_token));
        current_token.clear();
      }
      else if (ch == '"') inside_quotes = !inside_quotes;
      else current_token += ch;
    }
    tokens.push_back(trim(current_token));

    // Проверка кол-ва полей
    if (tokens.size() != 6) {
      error = "Строка " + std::to_string(line_num + 1) +
        ": ожидается 6 полей, найдено " + std::to_string(tokens.size());
      return {};
    }

    // Проверка пустых полей
    for (size_t i = 0; i < tokens.size(); ++i) {
      if (tokens[i].empty()) {
        error = "Строка " + std::to_string(line_num + 1) +
          ": поле " + std::to_string(i + 1) + " пустое";
        return {};
      }
    }

    // Проверка датф
    if (!is_valid_date(tokens[0])) {
      error = "Строка " + std::to_string(line_num + 1) +
        ": неверный формат даты (ожидается YYYY-MM-DD): " + tokens[0];
      return {};
    }

    // Проверка марки
    if (tokens[1].empty()) {
      error = "Строка " + std::to_string(line_num + 1) + ": марка бензина не может быть пустой";
      return {};
    }

    // Создаем структуру + парсим числа
    FuelPurchase p;
    p.date = tokens[0];
    p.brand = utf8_to_cp1251(tokens[1]);

    std::string field_names[] = { "одометр", "цена/л", "литры", "стоимость" }; // для ошибок
    double* field_ptr[] = { &p.odometer, &p.price_per_liter, &p.liters, &p.total_cost };

    for (int i = 0; i < 4; ++i) {
      int idx = i + 2;
      std::stringstream num_ss(tokens[idx]);
      num_ss.imbue(c_locale);

      double val;
      char leftover; // что после числа

      if (!(num_ss >> val)) {
        error = "Строка " + std::to_string(line_num + 1) +
          ": неверное число в поле '" + field_names[i] + "': " + tokens[idx];
        return {};
      }

      if (num_ss >> leftover) {
        error = "Строка " + std::to_string(line_num + 1) +
          ": мусор после числа в поле '" + field_names[i] + "': " + tokens[idx];
        return {};
      }

      *field_ptr[i] = val;
    }

    // Проверка на отрицательные
    if (p.odometer < 0 || p.price_per_liter < 0 || p.liters < 0 || p.total_cost < 0) {
      error = "Строка " + std::to_string(line_num + 1) + ": отрицательные значения не допускаются";
      return {};
    }

    // Проверяем логику
    double calculated_cost = p.price_per_liter * p.liters;
    if (std::abs(calculated_cost - p.total_cost) > 0.01) {
      error = "Строка " + std::to_string(line_num + 1) +
        ": цена * литры != стоимость (" +
        std::to_string(p.price_per_liter) + " * " +
        std::to_string(p.liters) + " = " +
        std::to_string(calculated_cost) +
        ", а стоимость " + std::to_string(p.total_cost) + ")";
      return {};
    }

    res.push_back(p);
  }

  // Проверка логики датф и одометра (должны расти)
  for (size_t i = 1; i < res.size(); ++i) {
    int days = days_between(res[i - 1].date, res[i].date);
    if (days < 0) {
      error = "Строка " + std::to_string(i + 1) +
        ": дата " + res[i].date + " раньше предыдущей " + res[i - 1].date;
      return {};
    }
    if (res[i].odometer < res[i - 1].odometer) {
      error = "Строка " + std::to_string(i + 1) +
        ": одометр " + std::to_string(res[i].odometer) +
        " меньше предыдущего " + std::to_string(res[i - 1].odometer);
      return {};
    }
  }

  return res;
}

Statistics compute_statistics(const std::vector<FuelPurchase>& data) {
  Statistics s = {};
  s.count = static_cast<int>(data.size());
  if (s.count == 0) return s;

  for (size_t i = 0; i < data.size(); ++i) {
    const auto& p = data[i];
    s.total_cost += p.total_cost;
    s.total_liters += p.liters;

    if (i > 0) {
      double km = p.odometer - data[i - 1].odometer;
      if (km > 0) {
        s.total_km += km;
        if (p.liters > 0)
          s.avg_km_per_liter += km / p.liters;
        s.avg_cost_per_km += p.total_cost / km;
      }

      double days = days_between(data[i - 1].date, p.date);
      s.total_days += days;

      if (days > 0)
        s.avg_cost_per_day += p.total_cost / days;

      double gallons = p.liters / GALLON_TO_LITER;
      if (gallons > 0 && days > 0)
        s.avg_days_per_gallon += days / gallons;
    }
  }

  int n = s.count - 1;
  if (n > 0) {
    s.avg_km_per_liter /= n;
    s.avg_cost_per_km /= n;
    s.avg_cost_per_day /= n;
    s.avg_days_per_gallon /= n;
  }
  s.avg_cost_per_gallon = (s.total_liters > 0) ? (s.total_cost / (s.total_liters / GALLON_TO_LITER)) : 0;

  return s;
}

std::map<std::string, Statistics> compute_brand_statistics(const std::vector<FuelPurchase>& data) {
  std::map<std::string, std::vector<FuelPurchase>> brand_data;
  for (const auto& p : data)
    brand_data[p.brand].push_back(p);

  std::map<std::string, Statistics> res;
  for (const auto& [brand, purchases] : brand_data)
    res[brand] = compute_statistics(purchases);
  return res;
}

Statistics compute_period_statistics(const std::vector<FuelPurchase>& data, int start_idx, int end_idx) {
  if (start_idx < 0 || end_idx >= static_cast<int>(data.size()) || start_idx > end_idx)
    return Statistics{};
  std::vector<FuelPurchase> subset;
  for (int i = start_idx; i <= end_idx; ++i)
    subset.push_back(data[i]);
  return compute_statistics(subset);
}