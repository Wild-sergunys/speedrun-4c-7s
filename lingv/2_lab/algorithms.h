#pragma once

#include <string>
#include <vector>
#include <map>

// Константы
constexpr double GALLON_TO_LITER = 3.78541;

/**
 * @brief Структура, описывающая одну запись о покупке бензина.
 */
struct FuelPurchase {
  // Входные данные (из файла)
  std::string date;              ///< Дата заправки (YYYY-MM-DD)
  std::string brand;             ///< Марка бензина
  double odometer = 0.0;         ///< Показания одометра (км)
  double price_per_liter = 0.0;  ///< Цена за один литр (руб)
  double liters = 0.0;           ///< Количество купленных литров
  double total_cost = 0.0;       ///< Общая стоимость заправки (руб)

  // Производные показатели (при выводе)
  double km_since_last = 0.0;    ///< Пробег с предыдущей заправки (км)
  double km_per_liter = 0.0;     ///< Расход топлива (сколько км на 1 литр)
  double cost_per_km = 0.0;      ///< Стоимость одного километра пути (руб/км)
  double cost_per_gallon = 0.0;  ///< Стоимость одного галлона (руб)
  double cost_per_day = 0.0;     ///< Средняя стоимость дня между заправками (руб/день)
  double days_since_last = 0.0;  ///< Количество дней с предыдущей заправки
  double gallons = 0.0;          ///< Количество галлонов в этой заправке
  double days_per_gallon = 0.0;  ///< Сколько дней хватает одного галлона
};

/**
 * @brief Структура для хранения статистических показателей.
 */
struct Statistics {
  double total_km = 0.0;              ///< Общий пробег (км)
  double total_liters = 0.0;          ///< Всего литров
  double total_cost = 0.0;            ///< Общая стоимость (руб)
  double total_days = 0.0;            ///< Общее количество дней
  double avg_km_per_liter = 0.0;      ///< Средний расход (км/л)
  double avg_cost_per_km = 0.0;       ///< Средняя стоимость 1 км (руб)
  double avg_cost_per_gallon = 0.0;   ///< Средняя стоимость галлона (руб)
  double avg_cost_per_day = 0.0;      ///< Средняя стоимость дня (руб)
  double avg_days_per_gallon = 0.0;   ///< Среднее дней на галлон
  int count = 0;                      ///< Количество заправок
};

void print_purchase(const FuelPurchase& p, const FuelPurchase* prev);
void print_statistics(const Statistics& s, const std::string& label);
std::vector<FuelPurchase> parse_fuel_data(const std::vector<std::string>& lines, std::string& error);
Statistics compute_statistics(const std::vector<FuelPurchase>& data);
std::map<std::string, Statistics> compute_brand_statistics(const std::vector<FuelPurchase>& data);
Statistics compute_period_statistics(const std::vector<FuelPurchase>& data, int start_idx, int end_idx);