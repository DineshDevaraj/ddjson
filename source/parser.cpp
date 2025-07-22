
#include "forward_declarations.h"
#include "node.h"
#include "parser.h"
#include "lexer.h"

namespace ddjson
{
   Node_t * Parser_t::ParseNode(Lexer_t &lex, Symbol node_close)
   {
      Node_t *temp = nullptr;
      Node_t *node = new Node_t();
      
      switch(lex.cur_sym)
      {
         case LEX_NEG         : 
         case LEX_INT         : const char *val;
                                if(LEX_INT == lex.get_num(val))
                                {
                                    node->vint = atoi(val);
                                    node->vtype = Valtype::Int;
                                }
                                else 
                                {
                                    node->vreal = atof(val);
                                    node->vtype = Valtype::Float;
                                }
                                break;

         case LEX_STRING      : lex.get_str(node->vstr);
                                if(LEX_STRING != lex.cur_sym)
                                {
                                    trw_err("Unterminated string value");
                                }
                                else 
                                { 
                                    node->vtype = Valtype::String;
                                    lex.next();
                                    break; 
                                }

         case LEX_BOOL_TRUE   : node->vbool = true;
                                node->vtype = Valtype::Bool;
                                lex.next();
                                break;
         
         case LEX_BOOL_FALSE  : node->vbool = false;
                                node->vtype = Valtype::Bool;
                                lex.next();
                                break;

         case LEX_NULL        : node->vtype = Valtype::Null;
                                lex.next();
                                break;

         case LEX_ARRAY_OPEN  : temp = this->ParseArray(lex);
                                node->vtype = Valtype::Array;
                                node->varr = temp;
                                lex.next(); /* move past array close symbol */
                                break;

         case LEX_OBJECT_OPEN : temp = this->ParseObject(lex);
                                node->vtype = Valtype::Object;
                                node->vobj = temp;
                                lex.next(); /* move past object close symbol */
                                break;

         default : trw_err("Expected number, char, string, array or object");
      }

      if(LEX_VALUE_SEPERATOR != lex.cur_sym and 
            node_close != lex.cur_sym)
         trw_err("Expected value seperator");

      return node;
   }

   Node_t * Parser_t::ParseArray(Lexer_t &lex)
   {
      Node_t *node = new Node_t();
      node->vtype = Valtype::Array;

      /* handle empty array */
      if(LEX_ARRAY_CLOSE == lex.next())
      {         
         node->vlast = NULL;
         node->varr = NULL;
         return node;
      }

      Node_t *curr = nullptr;
      while(LEX_ARRAY_CLOSE != lex.cur_sym)
      {
         Node_t *temp = this->ParseNode(lex, LEX_ARRAY_CLOSE);
         if(curr == nullptr) {
            node->vobj = temp;
         } else {
            curr->pnext = temp;
         }
         temp->pprev = curr;
         curr = temp;
         lex.next();
      }

      node->vlast = curr;

      return node;
   }

   Node_t * Parser_t::ParseObject(Lexer_t &lex)
   {
      Node_t *curr = nullptr;
      Node_t *node = new Node_t();

      while(LEX_OBJECT_CLOSE != lex.cur_sym)
      {
         /* handle empty objects */
         if(LEX_OBJECT_CLOSE == lex.next())
            break;

         if(LEX_STRING != lex.cur_sym)
            trw_err("Expected node name");

         lex.get_str(node->name);
         if(LEX_STRING != lex.cur_sym)
            trw_err("Invalid node name");

         if(LEX_NAME_SEPERATOR != lex.next())
            trw_err("Expected name seperator");
         
         lex.next();
         Node_t *temp = this->ParseNode(lex, LEX_OBJECT_CLOSE);
         temp->pprev = curr;
         if(curr == nullptr)
         {
            node->vobj = temp;
         }
         else
         {
            curr->pnext = temp;
         }
         curr = temp;
      }

      if(curr == node) /* object is empty */
      {
         node->vlast = node->vobj = nullptr;
      }
      else
      {
         node->vlast = curr;
         node->vlast->pnext = nullptr;
      }

      return node;
   }
}