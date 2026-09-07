/*
  lexical_analizer.h
  »нтерфейс лексического анализатора
*/ 

#pragma once

// “ипы лексем, используемых калькул€тором
enum  LexemType {
  LT_Unknown,
  LT_Number, 
  LT_End,
  LT_Delimiter,
  LT_Identifier,
  LT_Label,
  LT_String,
  LT_EOL,
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
  Parser(const Parser&);
  Parser& operator = (const Parser&);
public:
  class Holder {
    std::istream::pos_type  pos;
    unsigned int            line_number;
    friend class Parser;
  };
  std::istream& in;
  Lexem         last;
  int           line_number;
public:  
  Parser(std::istream& str);
  Lexem get_lexem();            // считать следующую лексему
  Lexem get_last();             // вернуть предыдущую лексему

  bool  SkipUntilEOL();

  Holder  Hold() const;
  void    Fetch(Holder);
  void    Reset();
};

