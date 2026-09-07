/*
  lexical_analizer.h
  »нтерфейс лексического анализатора
*/ 

#pragma once

// “ипы лексем, используемых калькул€тором
enum  LexemType {
  LT_Unknown,        // тип ещЄ не определЄн
  LT_Number,         // число
  LT_End,            // конец входного файла
  LT_Delimiter,      // знак: + - * / ( ) ; = < >
  LT_Identifier,     // им€: LET, PRINT, A, I
  LT_Label,          // метка
  LT_String,         // строка в кавычках
  LT_EOL,            // конец строки
};

// —труктура лексемы
struct Lexem {
  LexemType   type;         // тип
  float       value;        // значение    (если тип TT_Number)
  char        delimiter;    // разделитель (если тип TT_Delimiter)
  std::string name;         // им€         (если тип TT_Identifier)
};

// ѕарсер входного текста, раздел€ющий его на лексемы
class Parser {
  // ≈сть опасность копировани€ содержимого класса конструктором
  // копии или оператором присваивани€, сгенерированными по умолчанию
  // при неправильном использовании объектов класса Parser.
  // ќбъ€вл€€ их в закрытой части класса Parser € подавл€ю возможность
  // автоматической генерации таких методов.
  Parser(const Parser&);              // копирование запрещено
  Parser& operator = (const Parser&); // присваивание запрещено
public:
  class Holder {                      // снапшот позиции в файле
    std::istream::pos_type  pos;      // смещение в потоке
    unsigned int            line_number; // номер строки на этот момент
    friend class Parser;              // Parser может читать эти пол€
  };
  std::istream& in;                   // входной поток со скриптом
  Lexem         last;                 // последн€€ прочитанна€ лексема
  int           line_number;          // текущий номер строки
public:
  Parser(std::istream& str);          // прив€зать парсер к потоку
  Lexem get_lexem();                  // прочитать следующую лексему
  Lexem get_last();                   // вернуть уже прочитанную лексему

  bool  SkipUntilEOL();               // пропустить всЄ до конца строки

  Holder  Hold() const;               // запомнить текущую позицию
  void    Fetch(Holder);              // вернутьс€ к сохранЄнной позиции
  void    Reset();                    // перемотать поток в начало
};

