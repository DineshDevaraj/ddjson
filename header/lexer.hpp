
/* 
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#pragma once

#include <iostream>

namespace ddjson {
using namespace std;

#define SKIP_WHITE_SPACE(str, line, line_bgn) \
  ({                                          \
    for (; IS_SPACE(*str); str++)             \
      if ('\n' == *str) {                     \
        line++;                               \
        line_bgn = str + 1;                   \
      }                                       \
  })

#define IS_SPACE(ch) (' ' == ch or '\t' == ch or '\r' == ch or '\n' == ch)

struct DDJException {
  int line;
  std::string func;
  std::string file;
  std::string desc = "None";

  DDJException(const char *file, const char *func, size_t line,
               const char *desc)
      : file(file), func(func), line(line), desc(desc) {}
};

#define throw_ddjex(desc) \
  throw DDJException(__FILE__, __FUNCTION__, __LINE__, desc)

enum class Symbols {
  Int = 'I',
  Neg = '-',
  Float = '.',
  Null = 'N',
  True = 'T',
  False = 'F',
  DoubleQuote = '"',

  ArrayOpen = '[',
  ArrayClose = ']',

  ObjectOpen = '{',
  ObjectClose = '}',

  Colon = ':',
  Comma = ',',

  Invalid = 0x00
};

struct Lexer_t {
  Lexer_t();

  Symbols cur_sym;
  const char *cur_pos;
  const char *json_str;

  int line;
  const char *line_bgn;

  void load_string(const char *json_arg);

  Symbols next();
  Symbols get_sym();

  Symbols get_str(std::string &val);
  Symbols get_num(const char *&val);
};
}  // namespace ddjson