#pragma once

#include <string>

int get_int();
bool get_yes_no();
std::string get_file_path_txt();
bool save_res_to_file(const std::string& data);
std::string utf8_to_cp1251(const std::string& utf8_str);
