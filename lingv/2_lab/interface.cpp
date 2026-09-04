#include "interface.h"
#include "algorithms.h"
#include "utils.h"
#include "tests.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <map>

void show_help() {
  std::cout << "\nФормат данных (CSV, разделитель запятая):\n";
  std::cout << "дата, марка, одометр, цена_за_литр, литры, стоимость\n";
  std::cout << "Пример: 2026-01-15,АИ-92,15000,45.5,40,1820.0\n\n";
}

std::vector<std::string> read_file_lines(const std::string& path) {
  std::ifstream file(path);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line))
    lines.push_back(line);
  return lines;
}

void run_analysis() {
  std::cout << "\n---- Анализ покупок бензина ----\n"
            << "Загрузите файл с данными (CSV).\n";

  std::string path = get_file_path_txt();
  auto lines = read_file_lines(path);

  if (lines.empty()) {
    std::cout << "Ошибка: файл пуст.\n";
    return;
  }

  std::string error;
  auto data = parse_fuel_data(lines, error);
  if (!error.empty()) {
    std::cout << "Ошибка: " << error << "\n";
    return;
  }

  if (data.empty()) {
    std::cout << "Нет данных для анализа.\n";
    return;
  }

  std::cout << "\n---- Исходные данные с показателями ----\n";
  for (size_t i = 0; i < data.size(); ++i)
    print_purchase(data[i], (i > 0) ? &data[i - 1] : nullptr);

  auto total_stats = compute_statistics(data);
  print_statistics(total_stats, "Общая статистика за всё время");

  auto brand_stats = compute_brand_statistics(data);
  if (!brand_stats.empty()) {
    std::cout << "\n---- Статистика по маркам бензина ----\n";
    for (const auto& pair : brand_stats)
      print_statistics(pair.second, pair.first);
  }

  std::cout << "\nХотите посмотреть статистику за определённый период?";
  if (get_yes_no()) {
    std::cout << "Введите дату начала (YYYY-MM-DD): ";
    std::string start_date = get_date_from_user();

    std::string end_date;
    for (;;) {
      std::cout << "Введите дату конца (YYYY-MM-DD): ";
      end_date = get_date_from_user();
      if (end_date >= start_date) break;
      else std::cout << "Ошибка: дата конца должна быть >= даты начала.\n";
    }

    std::vector<FuelPurchase> period_data;
    for (const auto& p : data) 
      if (p.date >= start_date && p.date <= end_date) 
        period_data.push_back(p);

    if (period_data.empty())
      std::cout << "За период с " << start_date << " по " << end_date << " записей не найдено.\n";
    else {
      auto period_stats = compute_statistics(period_data);
      print_statistics(period_stats, "Статистика за период с " + start_date + " по " + end_date);
    }
  }

  std::cout << "\nСохранить результаты в файл?";
  if (get_yes_no()) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "---- СТАТИСТИКА ПОКУПОК БЕНЗИНА ----\n\n";
    ss << "Всего записей: " << data.size() << "\n";
    ss << "Общая стоимость: " << total_stats.total_cost << " руб.\n";
    ss << "Общий пробег: " << total_stats.total_km << " км\n";
    ss << "Средний расход: " << total_stats.avg_km_per_liter << " км/л\n";
    ss << "Средняя стоимость км: " << total_stats.avg_cost_per_km << " руб.\n";
    ss << "Средняя стоимость галлона: " << total_stats.avg_cost_per_gallon << " руб.\n";
    ss << "Средняя стоимость дня: " << total_stats.avg_cost_per_day << " руб.\n";
    ss << "Среднее дней на галлон: " << total_stats.avg_days_per_gallon << "\n";
    save_res_to_file(ss.str());
  }
}