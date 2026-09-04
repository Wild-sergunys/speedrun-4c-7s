#pragma once

#include <string>
#include <vector>

void show_help();
std::vector<std::string> read_file_lines(const std::string& path);
void run_analysis();
void run_tests();