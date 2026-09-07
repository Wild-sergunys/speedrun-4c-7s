/*
  lexical_analizer.cpp 
  Лексический анализатор.
  Модуль отвечает за выделение лексем из входного текста.
*/ 

#include "stdafx.h"
#include "lexical_analizer.h"

// Конструктор: запоминаем поток и начинаем с первой строки
Parser::Parser(std::istream& in)
: in(in), line_number(1) {            // инициализации членов
  last.type = LT_Unknown;             // лексемы ещё не читали
  in >> std::noskipws;                // не пропускать пробелы автоматически, istream их пропускает по умолчанию
}

// Возвращает предыдущую считанную лексему
// Если не было считано никакой лексемы - вызывает get_lexem
// для чтения лексемы
Lexem Parser::get_last()  {
  if (last.type==LT_Unknown)
    return get_lexem();
  return last;
}

// Чтение лексемы из потока
// Прочитать из потока одну следующую лексему
Lexem Parser::get_lexem()
{
  char c;
  in >> c;                            // взять первый символ

  // Пропустить пробелы и табы
  while (c == ' ' || c == '\t') {
    in >> c;
    if (!in)                          // файл кончился посреди пробелов
      break;
  }

  // Конец строки
  if (c == '\n') {
    last.type = LT_EOL;
    ++line_number;                    // следующая строка скрипта
    // Отладочная печать номеров строк
    //std::cout << "[" << line_number << "]";
    return last;
  }

  if (!in) {                          // поток исчерпан
    last.type = LT_End;
    return last;
  }

  // Строка в кавычках: "текст"
  if (c == '\"') {
    last.name = "";
    in >> c;
    while (c != '\"') {               // копируем символы до закрывающей кавычки
      last.name += c;
      in >> c;
      if (c == '\n' || !in)           // кавычку не закрыли до конца строки
        throw "unpaired quotation marks";
    }
    last.type = LT_String;
    return last;
  }

  // Односимвольные разделители: + - * / ( ) ; = < >
  const std::string delimiters("+-*/();=<>");
  std::string::size_type pos = delimiters.find(c);

  if (pos != std::string::npos) {     // символ найден в списке
    last.type = LT_Delimiter;
    last.delimiter = c;
    return last;
  }

  // Число: 10, 1.5, .5
  if (isdigit(c) || c == '.') {
    in.putback(c);                    // вернуть первый символ в поток
    in >> last.value;                 // istream сам разберёт float
    last.type = LT_Number;
    return last;
  }

  // Идентификатор: только буквы (LET, PRINT, A, I)
  if (isalpha(c)) {
    last.name = "";
    while (isalpha(c)) {              // копить буквы подряд
      last.name += c;
      in >> c;
    }
    in.putback(c);                    // первый небуквенный символ вернуть назад
    last.type = LT_Identifier;
    return last;
  }

  throw "Unexpected lexem";           // символ не подходит ни под один тип
}

// Снапшот текущей позиции в файле и номера строки
Parser::Holder Parser::Hold() const
{
  Holder holder;
  holder.pos = in.tellg();            // смещение каретки в потоке
  holder.line_number = line_number;   // текущая строка
  return holder;
}

// Перемотать поток на сохранённую позицию (FOR/GOTO)
void Parser::Fetch(Holder holder)
{
  in.seekg(holder.pos);  // переход на позицию
  line_number = holder.line_number;
}

// В начало файла - второй проход после сбора меток
void Parser::Reset() {
  in.clear();                         // снять флаг EOF после первого прохода
  in.seekg(0);                        // курсор на первый байт
}

// Пропустить все лексемы до конца текущей строки (для ложного IF)
bool Parser::SkipUntilEOL() {
  while (get_lexem().type != LT_EOL) {
    if (get_lexem().type == LT_End)   // файл кончился раньше конца строки
      return false;
  }
  get_lexem();                        // перейти к первой лексеме следующей строки
  return true;
}