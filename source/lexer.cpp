
#include <cstring>

#include "lexer.h"

using namespace ddjson;

Lexer_t::Lexer_t()
{
   line = 1;
   line_bgn = NULL;
   cur_pos = NULL;
   json_str = NULL;
}

void Lexer_t::load_string(const char *json_arg)
{
   line_bgn = json_str = cur_pos = json_arg;
   get_sym(); /* this will set cur_sym */
}

Symbol Lexer_t::next()
{
   cur_pos++;
   return get_sym();
}

Symbol Lexer_t::get_sym()
{
   SKIP_WHITE_SPACE(cur_pos, line, line_bgn);
   char ch = *cur_pos;
   switch(ch)
   {
      case LEX_NEG             :
      case LEX_STRING          :
      case LEX_ARRAY_OPEN      :
      case LEX_ARRAY_CLOSE     :
      case LEX_OBJECT_OPEN     :
      case LEX_OBJECT_CLOSE    :
      case LEX_NAME_SEPERATOR  :
      case LEX_VALUE_SEPERATOR : cur_sym = Symbol(ch);
                                 break;

      default  : if('0' <= ch and ch <= '9')
                    cur_sym = LEX_INT;
                 else if(0 == strncmp(cur_pos, "true", 4))
                 {
                    cur_sym  = LEX_BOOL_TRUE;
                    cur_pos += 3;
                 }
                 else if(0 == strncmp(cur_pos, "false", 5))
                 {
                    cur_sym  = LEX_BOOL_FALSE;
                    cur_pos += 4;
                 }
                 else if(0 == strncmp(cur_pos, "null", 4))
                 {
                    cur_sym  = LEX_NULL;
                    cur_pos += 3;
                 }
                 else cur_sym = LEX_INVALID;
   }
   return cur_sym;
}

Symbol Lexer_t::get_num(const char * &val)
{
   val = cur_pos;
   Symbol sym = LEX_INT;

   if('-' == *cur_pos and !isdigit(*++cur_pos))
         trw_err("Expected digit");

   while(isdigit(*cur_pos)) 
      cur_pos++;

   if('.' == *cur_pos && isdigit(cur_pos[1]))
   {
      sym = LEX_FLOAT;
      while(isdigit(*++cur_pos)); 
   }

   if('e' == *cur_pos || 'E' == *cur_pos)
   {
      sym = LEX_FLOAT;
      if(isdigit(*++cur_pos))
         while(isdigit(*++cur_pos));
      else if('+' == *cur_pos || '-' == *cur_pos)
      {
         if(not isdigit(*++cur_pos))
            trw_err("Expected digit");
         while(isdigit(*++cur_pos));
      }
   }

   get_sym();

   return sym;
}

Symbol Lexer_t::get_str(std::string &val)
{
   cur_pos++;           /* skip string symbol */
   val.clear();
   char arr[65] = {};

   for(int I = 0; '"' != *cur_pos; I++, cur_pos++)
   {
      if(I >= 64)
      {
         I = 0;
         val += arr;
         memset(arr, 0, sizeof arr);
      }

      if('\n' == *cur_pos)
      {
         line++;
         line_bgn = cur_pos + 1;
      }
      
      if('\\' == *cur_pos)
      {
         switch(*++cur_pos)
         {
            case '"'  : arr[I] = '"' ; break;
            case '\\' : arr[I] = '\\'; break;
            case '/'  : arr[I] = '/' ; break;
            case 'b'  : arr[I] = '\b'; break;
            case 'f'  : arr[I] = '\f'; break;
            case 'n'  : arr[I] = '\n'; break;
            case 'r'  : arr[I] = '\r'; break;
            case 't'  : arr[I] = '\t'; break;

            case 'u'  : arr[I] = 0;
                        for(int J = 0; J < 4 && cur_pos++; J++)
                        {
                           arr[I] <<= 4;
                           char ch = *cur_pos;
                           if('0' <= ch and ch <= '9')
                              arr[I] |= (ch - '0');
                           else if('a' <= ch and ch <= 'f')
                              arr[I] |= (ch - 'a' + 0xA);
                           else if('A' <= ch and ch <= 'F')
                              arr[I] |= (ch - 'A' + 0xA);
                           else trw_err("Invalid unicode value");
                        }
                        break;

            default   : trw_err("Invalid escape sequence");
         }
      }
      else
      {
         arr[I] = *cur_pos;
      }
   }
   
   val += arr;

   return get_sym();
}