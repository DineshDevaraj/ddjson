
#include "forward_declarations.h"
#include "node.h"
#include "parser.h"
#include "lexer.h"

namespace ddjson
{
   Node_t * Parser_t::ParseNode(Lexer_t &lex, Symbol node_close)
   {
      Node_t *node = nullptr;
      
      switch(lex.cur_sym)
      {
         case LEX_NEG         : 
         case LEX_INT         : 
            node = this->ParseNumbers(lex);
            break;
         case LEX_STRING      : 
            node = this->ParseString(lex);
            break;
         case LEX_BOOL_TRUE   : 
            node = this->ParseBool(lex, true);
            break;
         case LEX_BOOL_FALSE  : 
            node = this->ParseBool(lex, false);
            break;
         case LEX_NULL        : 
            node = this->ParseNull(lex);
            break;
         case LEX_ARRAY_OPEN  : 
            node = this->ParseArray(lex);
            break;
         case LEX_OBJECT_OPEN : 
            node = this->ParseObject(lex);
            break;

         default :
            std::string msg;
            std::string symbol = std::string("") + (char)lex.cur_sym;
            msg = R"(Expected number, char, string-open `"`,)";
            msg += " array-open `[` or object-open `{`";
            msg += " but got `" + symbol + "` instead";
            trw_err(msg.c_str());
      }

      if(LEX_VALUE_SEPERATOR != lex.cur_sym and 
            node_close != lex.cur_sym) 
      {
         char fstr[] = "Expected key-value seperator `:` or node close `%c` but got `%c` instead";
         char msg[sizeof(fstr)];
         sprintf(msg, fstr, node_close, lex.cur_sym);
         trw_err(msg);
      }

      return node;
   }

   Node_t * Parser_t::ParseNumbers(Lexer_t &lex)
   {
      const char *val;
      Node_t *node = new Node_t();

      if(LEX_INT == lex.get_num(val))
      {
         node->vtype = Valtype::Int;
         node->vint = atoi(val);
      }
      else 
      {
         node->vtype = Valtype::Float;
         node->vreal = atof(val);
      }

      return node;
   }

   Node_t * Parser_t::ParseString(Lexer_t &lex)
   {
      Node_t *node = new Node_t(Valtype::String);
      lex.get_str(node->vstr);
      if(LEX_STRING != lex.cur_sym)
         trw_err("Unterminated string value");
      lex.next();
      return node;
   }

   Node_t * Parser_t::ParseBool(Lexer_t &lex, bool val)
   {
      Node_t *node = new Node_t(Valtype::Bool);
      node->vbool = val;
      lex.next();
      return node;
   }

   Node_t * Parser_t::ParseNull(Lexer_t &lex)
   {
      Node_t *node = new Node_t(Valtype::Null);
      lex.next();
      return node;
   }

   Node_t * Parser_t::ParseArray(Lexer_t &lex)
   {
      Node_t *node = new Node_t(Valtype::Array);

      /* move past the array open symbol */
      lex.next();

      /* handle empty array */
      if(LEX_ARRAY_CLOSE != lex.cur_sym)
      {
         Node_t *curr;
         for(curr = nullptr; ; lex.next())
         {
            Node_t *temp = this->ParseNode(lex, LEX_ARRAY_CLOSE);
            NEXT_NEW_NODE(node, curr);
            if(LEX_ARRAY_CLOSE == lex.cur_sym)
               break;
         }
         node->vlast = curr;
      }

      /* move past array close symbol */
      lex.next();

      return node;
   }

   Node_t * Parser_t::ParseObject(Lexer_t &lex)
   {
      string name;
      Node_t *curr = nullptr;
      Node_t *node = new Node_t(Valtype::Object);

      while(LEX_OBJECT_CLOSE != lex.cur_sym)
      {
         /* handle empty objects */
         if(LEX_OBJECT_CLOSE == lex.next())
            break;

         if(LEX_STRING != lex.cur_sym)
            trw_err("Expected node name");

         lex.get_str(name);
         if(LEX_STRING != lex.cur_sym)
            trw_err("Invalid node name");

         if(LEX_NAME_SEPERATOR != lex.next())
            trw_err("Expected name seperator");
         
         lex.next();
         Node_t *temp = this->ParseNode(lex, LEX_OBJECT_CLOSE);
         temp->name = name;
         
         NEXT_NEW_NODE(node, curr);
      }

      node->vlast = curr;
      /* move past object close symbol */
      lex.next(); 

      return node;
   }
}