/*
  label_table.h
  Интерфейс таблицы меток переходов
*/ 

#pragma once

#include "lexical_analizer.h"

class LabelTable {
  class LabelTableImpl *pImpl;
public:
  LabelTable();
  ~LabelTable();
  void           AddLabel(unsigned int, Parser::Holder);
  Parser::Holder GetHolder(unsigned int);
};

extern LabelTable LT;