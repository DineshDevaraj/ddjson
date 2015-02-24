
/**
 *
 * Author   : D.Dinesh
 * Website  : www.techybook.com
 * Email Id : dinesh@techybook.com
 *
 * Created  : 19 Feb 2015 - Thu
 * Updated  : 20 Feb 2015 - Fri
 *
 * Licence : Refer the license file
 *
 **/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define OK   true
#define ERR  false

#define SKIP_WHITE_SPACE(str, line, line_bgn)   \
({                                              \
   for( ; IS_SPACE(*str); str++)                \
      if('\n' == *str)                          \
      {                                         \
         line++;                                \
         line_bgn = str + 1;                    \
      }                                         \
 })

#define IS_SPACE(ch) (' ' == ch or '\t' == ch or '\r' == ch or '\n' == ch)

struct Exception
{
   int line;
   const char *fn;
   const char *file;
   const char *msg;

   Exception(const char *file, const char *fn, size_t line, const char *msg) :
      file(file), fn(fn), line(line), msg(msg) {}
};

#define trw_err(msg) throw Exception(__FILE__, __FUNCTION__, __LINE__, msg)

namespace Lexer
{
   enum Symbol
   {
      LEX_INT              = 0x01   ,
      LEX_FLOAT                     ,
      LEX_CHAR             = '\''   ,
      LEX_STRING           = '"'    ,

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

      Symbol cur();
      Symbol next();
      Symbol get_sym();

      bool get_num(char *val);
      bool get_str(char *val);
      bool get_char(char &val);
   };

   Lexer_t::Lexer_t()
   {
      line = 1;
      line_bgn = NULL;
      cur_pos = NULL;
      json_str = NULL;
   }

   void Lexer_t::load_string(const char *json_arg)
   {
      line_bgn = json_str = 
         cur_pos = json_arg;
      get_sym(); /* this will set cur_sym */
   }

   Symbol Lexer_t::cur()
   {
      return cur_sym;
   }

   Symbol Lexer_t::next()
   {
      cur_pos++;
      get_sym();
      return cur_sym;
   }

   Symbol Lexer_t::get_sym()
   {
      SKIP_WHITE_SPACE(cur_pos, line, line_bgn);
      char ch = *cur_pos;
      switch(ch)
      {
         case LEX_CHAR            :
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
                    else cur_sym = LEX_INVALID;
      }
      return cur_sym;
   }

   bool Lexer_t::get_num(char *val)
   {
      while(isdigit(*cur_pos)) 
         *val++ = *cur_pos++;

      if('.' == *cur_pos && isdigit(cur_pos[1]))
      {
         *val++ = *cur_pos++;
         cur_sym = LEX_FLOAT;
         while(isdigit(*cur_pos)) 
            *val++ = *cur_pos++;
      }

      val[0] = 0;
      get_sym();

      return true;
   }

   bool Lexer_t::get_char(char &val)
   {
      cur_pos++; /* ship char symbol */
      val = *cur_pos++;
      get_sym();
      return true;
   }

   bool Lexer_t::get_str(char *val)
   {
      cur_pos++; /* skip string symbol */

      for( ; '"' != *cur_pos; val++, cur_pos++)
      {
         if('\\' == *cur_pos)
         {
            switch(*++cur_pos)
            {
               case '"'  : *val = '"' ; break;
               case '\\' : *val = '\\'; break;
               case '/'  : *val = '/' ; break;
               case 'b'  : *val = '\b'; break;
               case 'f'  : *val = '\f'; break;
               case 'n'  : *val = '\n'; break;
               case 'r'  : *val = '\r'; break;
               case 't'  : *val = '\t'; break;

               case 'u'  : *val = 0;
                           for(int I = 0; I < 4 && cur_pos++; I++)
                           {
                              *val <<= 4;
                              char ch = *cur_pos;
                              if('0' <= ch and ch <= '9')
                                 *val |= (ch - '0');
                              else if('a' <= ch and ch <= 'f')
                                 *val |= (ch - 'a' + 0xA);
                              else if('A' <= ch and ch <= 'F')
                                 *val |= (ch - 'A' + 0xA);
                              else trw_err("Invalid unicode value");
                           }
                           break;

               default   : trw_err("Invalid escape sequence");
            }
         }
         else
         {
            *val = *cur_pos;
         }
      }

      get_sym();
      return true;
   }
}

namespace Parser
{
   using namespace Lexer;

   bool ParseArray(Lexer_t &lex);
   bool ParseObject(Lexer_t &lex);

   bool ParseNode(Lexer_t &lex, Symbol node_close, char *val)
   {
      switch(lex.next())
      {
         case LEX_INT         : lex.get_num(val);
                                break;

         case LEX_CHAR        : lex.get_char(*val);
                                if(LEX_CHAR != lex.cur())
                                   trw_err("Unterminated char value");
                                else { lex.next(); break; }

         case LEX_STRING      : lex.get_str(val);
                                if(LEX_STRING != lex.cur())
                                   trw_err("Unterminated string value");
                                else { lex.next(); break; }

         case LEX_ARRAY_OPEN  : ParseArray(lex);
                                lex.next();        /* move past array close ] symbol */
                                break;

         case LEX_OBJECT_OPEN : ParseObject(lex);
                                lex.next();        /* move past object close } symbol */
                                break;

         default : trw_err("Expected number, char, string, array or object");
      }

      if(LEX_VALUE_SEPERATOR != lex.cur() and 
            node_close != lex.cur())
         trw_err("Expected value seperator");

      return OK;
   }

   bool ParseArray(Lexer_t &lex)
   {
      while(LEX_ARRAY_CLOSE != lex.cur())
      {
         char val[32] = {};
         ParseNode(lex, LEX_ARRAY_CLOSE, val);
         printf("%s, ", val);
      }
      return OK;
   }

   bool ParseObject(Lexer_t &lex)
   {
      while(LEX_OBJECT_CLOSE != lex.cur())
      {
         char val[32] = {};
         char name[32] = {};

         if(LEX_STRING != lex.next())
            trw_err("Expected node name");

         lex.get_str(name);
         if(LEX_STRING != lex.cur())
            trw_err("Invalid node name");

         if(LEX_NAME_SEPERATOR != lex.next())
            trw_err("Expected name seperator");

         ParseNode(lex, LEX_OBJECT_CLOSE, val);

         printf("%s : %s\n", name, val);
      }
      return OK;
   }

   void JsonParse(const char *json_arg)
   {
      Lexer_t lex;

      try
      {
         lex.load_string(json_arg);
         if(LEX_OBJECT_OPEN != lex.cur())
            trw_err("Expected object at start");
         ParseObject(lex);
      }
      catch(Exception exc)
      {
         printf("%s() %d : %s at line %d %.*s\n", exc.fn, exc.line, exc.msg, lex.line, lex.cur_pos - lex.line_bgn, lex.line_bgn);
      }

      return;
   }
}

using namespace Parser;

int main()
{
   char json_str[1024] = {};
   FILE *fh = fopen("Sample.json", "r");
   fread(json_str, sizeof(char), sizeof json_str, fh);
   JsonParse(json_str);
   return 0;
}
