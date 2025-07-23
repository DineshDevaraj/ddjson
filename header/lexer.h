
#pragma once

#include <iostream>

namespace ddjson
{
    using namespace std;
    
    #define SKIP_WHITE_SPACE(str, line, line_bgn)   \
    ({                                              \
    for( ; IS_SPACE(*str); str++)                   \
        if('\n' == *str)                            \
        {                                           \
            line++;                                 \
            line_bgn = str + 1;                     \
        }                                           \
    })

    #define IS_SPACE(ch) (' ' == ch or '\t' == ch or '\r' == ch or '\n' == ch)

   struct DDJException
   {
      int line;
      std::string func;
      std::string file;
      std::string desc = "None";

      DDJException(const char *file, const char *func, size_t line, const char *desc) :
         file(file), func(func), line(line), desc(desc) {}
   };

   #define throw_ddjex(desc) throw DDJException(__FILE__, __FUNCTION__, __LINE__, desc)

    enum Symbol
    {
        LEX_INT              = 'I'    ,
        LEX_NEG              = '-'    ,
        LEX_FLOAT            = '.'    ,
        LEX_STRING           = '"'    ,
        LEX_NULL             = 'N'    ,
        LEX_BOOL_TRUE        = 'T'    ,
        LEX_BOOL_FALSE       = 'F'    ,

        LEX_ARRAY_OPEN       = '['    ,
        LEX_ARRAY_CLOSE      = ']'    ,

        LEX_OBJECT_OPEN      = '{'    ,
        LEX_OBJECT_CLOSE     = '}'    ,

        LEX_NAME_SEPERATOR   = ':'    ,
        LEX_VALUE_SEPERATOR  = ','    ,

        LEX_INVALID          = 0x00
    };

    struct Lexer_t
    {
        Lexer_t();

        Symbol cur_sym;
        const char *cur_pos;
        const char *json_str;

        int line;
        const char *line_bgn;

        void load_string(const char *json_arg);

        Symbol next();
        Symbol get_sym();

        Symbol get_str(std::string &val);
        Symbol get_num(const char * &val);
    };
}