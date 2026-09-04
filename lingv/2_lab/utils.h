#pragma once

#include <string>
#include <vector>

int get_int();
double get_double();
bool get_yes_no();
std::string get_file_path_txt();
std::string get_date_from_user();
bool save_res_to_file(const std::string& data);
std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);
int days_between(const std::string& date1, const std::string& date2);
bool is_valid_date(const std::string& date);
std::string utf8_to_cp1251(const std::string& utf8_str);