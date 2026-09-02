#include "algorithms.h"
#include <cctype>

std::string find_words_with_char(const std::string& text, char ch) {
  std::string res;
  std::string word;

  for (char c : text) {
    if (std::isalpha(static_cast<unsigned char>(c)) || c == '-' || c == '\'')
      word += c;
    else {
      if (!word.empty()) {
        if (word.find(ch) != std::string::npos) {
          if (!res.empty())
            res += " ";
          res += word;
        }
        word.clear();
      }
    }
  }

  if (!word.empty() && word.find(ch) != std::string::npos) {
    if (!res.empty())
      res += " ";
    res += word;
  }

  return res;
}