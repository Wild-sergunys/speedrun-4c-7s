/*
  name_table.h
  Интерфейс таблицы имён интерпретатора
*/ 

#pragma once

#include "interface.h"

class NameTable {
  std::map<std::string, Command *> commands;
  std::map<std::string, float>     variables;
public:
  ~NameTable();
  // Регистрация команды языка в таблице имен
  void RegisterCommand(std::string const& name, Command *);
  // Выполнить команду, если это возможно
  bool ProcessCommand(Parser&);

  void  SetVariable(std::string const& name, float value);
  float GetVariable(std::string const& name);
};

extern NameTable NT;