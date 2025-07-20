
#include "forward_declarations.h"
#include "node.h"
#include "parser.h"
#include "lexer.h"

namespace ddjson
{
   bool Parser_t::ParseNode(Lexer_t &lex, Symbol node_close)
   {
      switch(lex.cur_sym)
      {
         case LEX_NEG         : 
         case LEX_INT         : const char *val;
                                if(LEX_INT == lex.get_num(val))
                                {
                                   vint = atoi(val);
                                   vtype = Valtype::Int;
                                }
                                else 
                                {
                                   vreal = atof(val);
                                   vtype = Valtype::Float;
                                }
                                break;

         case LEX_STRING      : lex.get_str(vstr);
                                if(LEX_STRING != lex.cur_sym)
                                {
                                   trw_err("Unterminated string value");
                                }
                                else 
                                { 
                                   lex.next(); 
                                   vtype = Valtype::String; 
                                   break; 
                                }

         case LEX_BOOL_TRUE   : vbool = true;
                                vtype = Valtype::Bool;
                                lex.next();
                                break;
         
         case LEX_BOOL_FALSE  : vbool = false;
                                vtype = Valtype::Bool;
                                lex.next();
                                break;

         case LEX_NULL        : vtype = Valtype::Null;
                                lex.next();
                                break;

         case LEX_ARRAY_OPEN  : ParseArray(lex);
                                vtype = Valtype::Array;
                                lex.next(); /* move past array close symbol */
                                break;

         case LEX_OBJECT_OPEN : ParseObject(lex);
                                vtype = Valtype::Object;
                                lex.next(); /* move past object close symbol */
                                break;

         default : trw_err("Expected number, char, string, array or object");
      }

      if(LEX_VALUE_SEPERATOR != lex.cur_sym and 
            node_close != lex.cur_sym)
         trw_err("Expected value seperator");

      return OK;
   }

   bool Parser_t::ParseArray(Lexer_t &lex)
   {
      Parser_t *pp = NULL;

      /* handle empty array */
      if(LEX_ARRAY_CLOSE == lex.next())
         return OK;

      pcount++;
      vobj = pp = new Parser_t(pdoc);
      pp->pparent = this;
      pp->ParseNode(lex, LEX_ARRAY_CLOSE);

      while(LEX_ARRAY_CLOSE != lex.cur_sym)
      {
         pcount++;
         lex.next();
         NEXT_NEW_NODE(pp);
         pp->ParseNode(lex, LEX_ARRAY_CLOSE);
      }

      vlast = pp;

      return OK;
   }

   bool Parser_t::ParseObject(Lexer_t &lex)
   {
      Parser_t *pp = NULL;
      vobj = pp = new Parser_t(pdoc);

      while(LEX_OBJECT_CLOSE != lex.cur_sym)
      {
         /* handle empty objects */
         if(LEX_OBJECT_CLOSE == lex.next())
            break;

         if(LEX_STRING != lex.cur_sym)
            trw_err("Expected node name");

         lex.get_str(pp->name);
         if(LEX_STRING != lex.cur_sym)
            trw_err("Invalid node name");

         if(LEX_NAME_SEPERATOR != lex.next())
            trw_err("Expected name seperator");
         
         pcount++;
         lex.next();
         pp->ParseNode(lex, LEX_OBJECT_CLOSE);
         NEXT_NEW_NODE(pp);
      }

      if(vobj == pp) /* object is empty */
      {
         vobj = NULL;
      }
      else
      {
         vlast = pp->pprev;
         vlast->pnext = NULL;
      }
      delete pp;

      return OK;
   }
}