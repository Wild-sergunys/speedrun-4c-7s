#include "tests.h"
#include "algorithms.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

void run_tests() {
  std::cout << "\n---- ЗАПУСК ТЕСТОВ ----\n";

  struct TestCase {
    std::vector<std::string> lines; // Входные данные
    bool expect_error;              // Ожиаем ли ошибку (t/f)
    int expected_count;             // Кол-во записей
    double expected_total_cost;     // Ожидаемая общая стоимость
    double expected_total_km;       // Ожидаемый общий пробег
  };

  std::vector<TestCase> tests;

  tests.push_back({
      {
          "2026-01-10,AI-92,1000,45.0,40,1800",
          "2026-01-15,AI-95,1150,48.5,35,1697.5",
          "2026-01-20,AI-92,1300,46.0,38,1748"
      },
      false, 3, 5245.5, 300.0
    });

  tests.push_back({ {}, true, 0, 0.0, 0.0 });

  tests.push_back({
      {"2026-01-10,AI-92,1000,-45.0,40,1800"},
      true, 0, 0.0, 0.0
    });

  tests.push_back({
      {"2026-01-10,AI-92,1000,45.0,40,1800"},
      false, 1, 1800.0, 0.0
    });

  tests.push_back({
      {"2026-01-10,AI-92,1000,45.0,40"},
      true, 0, 0.0, 0.0
    });

  tests.push_back({
      {
          "2026-01-10,AI-92,1000,45.0,40,1800",
          "2026-01-15,AI-92,5000,45.0,50,2250"
      },
      false, 2, 4050.0, 4000.0
    });

  int passed = 0;
  for (size_t i = 0; i < tests.size(); ++i) {
    const auto& test = tests[i];
    std::string error;
    auto data = parse_fuel_data(test.lines, error);

    bool ok = true;

    if ((!error.empty()) != test.expect_error) {
      std::cout << "FAIL Test " << i + 1 << ": error mismatch (got: '" << error << "')\n";
      ok = false;
    }
    else if ((int)data.size() != test.expected_count) {
      std::cout << "FAIL Test " << i + 1 << ": count " << data.size() << " != " << test.expected_count << "\n";
      ok = false;
    }
    else if (data.size() > 0) {
      auto stats = compute_statistics(data);
      if (std::abs(stats.total_cost - test.expected_total_cost) > 0.01) {
        std::cout << "FAIL Test " << i + 1 << ": cost " << stats.total_cost << " != " << test.expected_total_cost << "\n";
        ok = false;
      }
      if (std::abs(stats.total_km - test.expected_total_km) > 0.01) {
        std::cout << "FAIL Test " << i + 1 << ": km " << stats.total_km << " != " << test.expected_total_km << "\n";
        ok = false;
      }
    }

    if (ok) {
      std::cout << "PASS Test " << i + 1 << "\n";
      passed++;
    }
  }

  std::cout << "\nТестов пройдено: " << passed << " из " << tests.size() << "\n";
  if (passed == (int)tests.size())
    std::cout << "Тестирование прошло успешно.\n";
  else 
    std::cout << "Тестирование не прошло.\n";
}