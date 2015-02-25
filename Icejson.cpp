
/**
 *
 * Author   : D.Dinesh
 * Website  : www.techybook.com
 * Email Id : dinesh@techybook.com
 *
 * Created  : 19 Feb 2015 - Thu
 * Updated  : 25 Feb 2015 - Wed
 *
 * Licence : Refer the license file
 *
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "Icejson.h"

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

   Symbol get_num(char *val);
   Symbol get_str(char *val);
   Symbol get_char(char &val);
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

Symbol Lexer_t::get_num(char *val)
{
   Symbol sym = LEX_INT;

   while(isdigit(*cur_pos)) 
      *val++ = *cur_pos++;

   if('.' == *cur_pos && isdigit(cur_pos[1]))
   {
      *val++ = *cur_pos++;
      cur_sym = LEX_FLOAT;
      while(isdigit(*cur_pos)) 
         *val++ = *cur_pos++;
      sym = LEX_FLOAT;
   }

   val[0] = 0;
   get_sym();

   return sym;
}

Symbol Lexer_t::get_char(char &val)
{
   cur_pos++; /* ship char symbol */
   val = *cur_pos++;
   return get_sym();
}

Symbol Lexer_t::get_str(char *val)
{
   cur_pos++; /* skip string symbol */

   for( ; '"' != *cur_pos; val++, cur_pos++)
   {
      if('\n' == *cur_pos)
      {
         line++;
         line_bgn = cur_pos + 1;
      }
      else if('\\' == *cur_pos)
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

   return get_sym();
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
 |        Parser related implementations starts      |
 `~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
namespace Icejson
{
   static Icejson::Node_t oInvalid;

   struct Parser_t : public Node_t
   {
      Parser_t() {}

      Parser_t(Valuetype_t type) { vtype = type; }

      Parser_t * AddChild();
      bool ParseArray(Lexer_t &lex);
      bool ParseObject(Lexer_t &lex);
      bool ParseNode(Lexer_t &lex, Symbol node_close, char *val);
   };

   Parser_t * Parser_t::AddChild()
   {
      Parser_t *pp = new Parser_t;
      pp->pparent = this;
      vobj = pp;
      return pp;
   }

   bool Parser_t::ParseNode(Lexer_t &lex, Symbol node_close, char *val)
   {
      switch(lex.next())
      {
         case LEX_INT         : if(LEX_INT == lex.get_num(val))
                                {
                                   vint = atoi(val);
                                   vtype = Valuetype::Int;
                                }
                                else 
                                {
                                   vreal = atof(val);
                                   vtype = Valuetype::Float;
                                }
                                break;

         case LEX_CHAR        : lex.get_char(*val);
                                if(LEX_CHAR != lex.cur())
                                   trw_err("Unterminated char value");
                                else { lex.next(); vtype = Valuetype::Char; break; }

         case LEX_STRING      : lex.get_str(val);
                                if(LEX_STRING != lex.cur())
                                   trw_err("Unterminated string value");
                                else { lex.next(); vtype = Valuetype::String; break; }

         case LEX_ARRAY_OPEN  : AddChild()->ParseArray(lex);
                                vtype = Valuetype::Array;
                                lex.next(); /* move past array close symbol */
                                break;

         case LEX_OBJECT_OPEN : AddChild()->ParseObject(lex);
                                vtype = Valuetype::Object;
                                lex.next(); /* move past object close symbol */
                                break;

         default : trw_err("Expected number, char, string, array or object");
      }

      if(LEX_VALUE_SEPERATOR != lex.cur() and 
            node_close != lex.cur())
         trw_err("Expected value seperator");

      return OK;
   }

   bool Parser_t::ParseArray(Lexer_t &lex)
   {
      while(LEX_ARRAY_CLOSE != lex.cur())
      {
         char val[32] = {};
         vtype = Valuetype::Array;
         ParseNode(lex, LEX_ARRAY_CLOSE, val);
         printf("%s, ", val);
      }
      return OK;
   }

   bool Parser_t::ParseObject(Lexer_t &lex)
   {
      Parser_t *pp = NULL;
      vobj = pp = new Parser_t;

      while(LEX_OBJECT_CLOSE != lex.cur())
      {
         char sval[32] = {};
         char sname[32] = {};

         if(LEX_OBJECT_CLOSE == lex.next())
            break;

         if(LEX_STRING != lex.cur())
            trw_err("Expected node name");

         lex.get_str(sname);
         if(LEX_STRING != lex.cur())
            trw_err("Invalid node name");

         if(LEX_NAME_SEPERATOR != lex.next())
            trw_err("Expected name seperator");
         
         pp->name = sname;
         pp->pparent = this;

         pp->ParseNode(lex, LEX_OBJECT_CLOSE, sval);

         pp->vstr = sval;
         
         printf("%s : %s\n", pp->name.data(), sval);

         Parser_t *swp = new Parser_t;
         swp->pprev = pp;
         pp->pnext = swp;
         pp = swp;
      }

      pp->pprev->pnext = NULL;
      delete pp;

      return OK;
   }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
 |        Document related implementations starts      |
 `~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
namespace Icejson
{
   Doc_t::Doc_t() {}

   Node_t & Doc_t::parse_string(const char *json_arg)
   {
      Lexer_t lex;
      Parser_t *pp = NULL; /* pointer to parser */

      try
      {
         lex.load_string(json_arg);

         if(LEX_OBJECT_OPEN != lex.cur())
            trw_err("Expected object at start");

         pp = new Parser_t(Valuetype::Object);
         pp->ParseObject(lex);

         return *pp;
      }
      catch(Exception exc)
      {
         error.desc = exc.msg;
         error.line = lex.line;
         error.colum = lex.cur_pos - lex.line_bgn;
         error.offset = lex.cur_pos - lex.json_str;
      }

      return oInvalid;
   }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
 |       Node related implementations starts      |
 `~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
namespace Icejson
{
   Node_t::Node_t()    
   {
      pdoc = NULL;

      proot = NULL;
      pnext = NULL;
      pprev = NULL;
      pparent = NULL;

      vobj = NULL;

      vtype = Valuetype::Invalid;
   }

   Node_t::operator bool ()
   {
      return this != &oInvalid;
   }

   bool Node_t::valid()
   {
      return this != &oInvalid;
   }

   Iterator_t Node_t::begin()
   {
      if(Valuetype::Array == vtype or
            Valuetype::Object == vtype)
         return Iterator_t(vobj);
      return Iterator_t(NULL);
   }

   Iterator_t Node_t::end()
   {
      return Iterator_t(NULL);
   }

   Valuetype_t Node_t::get_value_type()
   {
      return vtype;
   }

   template <> int Node_t::value()
   {
      return vint;
   }

   template <> float Node_t::value()
   {
      return vreal;
   }

   template <> char Node_t::value()
   {
      return vchar;
   }

   template <> string Node_t::value()
   {
      return vstr;
   }

   template <> Node_t & Node_t::value()
   {
      return *vobj;
   }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.
 |        Iterator related implementations starts      |
 `~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
namespace Icejson
{
   Iterator_t::Iterator_t() {}

   Iterator_t::Iterator_t(Node_t *node) :
      pcur(node) {}

   Node_t & Iterator_t::operator * ()
   {
      return *pcur;
   }

   Iterator_t & Iterator_t::operator ++ ()
   {
      pcur = pcur->pnext;
      return *this;
   }

   Iterator_t & Iterator_t::operator -- ()
   {
      pcur = pcur->pprev;
      return *this;
   }

   Iterator_t Iterator_t::operator ++ (int)
   {
      Iterator_t itr(pcur);
      pcur = pcur->pnext;
      return itr;
   }

   Iterator_t Iterator_t::operator -- (int)
   {
      Iterator_t itr(pcur);
      pcur = pcur->pprev;
      return itr;
   }

   bool Iterator_t::operator == (const Iterator_t &rhs)
   {
      return pcur == rhs.pcur;
   }

   bool Iterator_t::operator != (const Iterator_t &rhs)
   {
      return pcur != rhs.pcur;
   }
}
