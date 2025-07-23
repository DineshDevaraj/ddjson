
#include "forward_declarations.h"
#include "parser.h"
#include "helper.h"

namespace ddjson
{
   Parser_t::Parser_t(const char *json_str) {
      this->lex.load_string(json_str);
   }

   Node_t * Parser_t::ParseRoot() {
      try
      {
         if(LEX_OBJECT_OPEN != this->lex.cur_sym)
            trw_err("Expected object at start");
         this->proot = this->ParseObject();
         return this->proot;
      }
      catch(Exception exc)
      {
         error.desc = exc.msg;
         error.line = this->lex.line;
         error.colum = this->lex.cur_pos - this->lex.line_bgn + 1;
         error.offset = this->lex.cur_pos - this->lex.json_str + 1;
      }
   }

   Node_t & Parser_t::root() {
      return *this->proot;
   }

   Node_t * Parser_t::ParseNode(Symbol node_close)
   {
      Node_t *node = nullptr;
      
      switch(this->lex.cur_sym)
      {
         case LEX_NEG         : 
         case LEX_INT         : 
            node = this->ParseNumbers();
            break;
         case LEX_STRING      : 
            node = this->ParseString();
            break;
         case LEX_BOOL_TRUE   : 
            node = this->ParseBool(true);
            break;
         case LEX_BOOL_FALSE  : 
            node = this->ParseBool(false);
            break;
         case LEX_NULL        : 
            node = this->ParseNull();
            break;
         case LEX_ARRAY_OPEN  : 
            node = this->ParseArray();
            break;
         case LEX_OBJECT_OPEN : 
            node = this->ParseObject();
            break;

         default :
            std::string msg;
            std::string symbol = std::string("") + (char)this->lex.cur_sym;
            msg = R"(Expected number, char, string-open `"`,)";
            msg += " array-open `[` or object-open `{`";
            msg += " but got `" + symbol + "` instead";
            trw_err(msg.data());
      }

      if(LEX_VALUE_SEPERATOR != this->lex.cur_sym and 
            node_close != this->lex.cur_sym) 
      {
         std::string close_symbol = std::string("") + (char)node_close;
         std::string curr_symbol = std::string("") + (char)this->lex.cur_sym;
         std::string msg = "Expected name-value separator `:`, value-value separator `,`";
         msg += " or close-symbol `" + close_symbol + "` but got `";
         msg += curr_symbol + "` instead";
         trw_err(msg.data());
      }

      return node;
   }

   Node_t * Parser_t::ParseNumbers()
   {
      const char *val;
      Node_t *node = new Node_t();

      if(LEX_INT == this->lex.get_num(val))
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

   Node_t * Parser_t::ParseString()
   {
      Node_t *node = new Node_t(Valtype::String);
      this->lex.get_str(node->vstr);
      if(LEX_STRING != this->lex.cur_sym)
         trw_err("Unterminated string value");
      this->lex.next();
      return node;
   }

   Node_t * Parser_t::ParseBool(bool val)
   {
      Node_t *node = new Node_t(Valtype::Bool);
      node->vbool = val;
      this->lex.next();
      return node;
   }

   Node_t * Parser_t::ParseNull()
   {
      Node_t *node = new Node_t(Valtype::Null);
      this->lex.next();
      return node;
   }

   Node_t * Parser_t::ParseArray()
   {
      Node_t *node = new Node_t(Valtype::Array);

      /* move past the array open symbol */
      this->lex.next();

      /* handle empty array */
      if(LEX_ARRAY_CLOSE != this->lex.cur_sym)
      {
         Node_t *curr;
         for(curr = nullptr; ; this->lex.next())
         {
            Node_t *temp = this->ParseNode(LEX_ARRAY_CLOSE);
            NEXT_NEW_NODE(node, curr);
            if(LEX_ARRAY_CLOSE == this->lex.cur_sym)
               break;
         }
         node->vlast = curr;
      }

      /* move past array close symbol */
      this->lex.next();

      return node;
   }

   Node_t * Parser_t::ParseObject()
   {
      string name;
      Node_t *curr = nullptr;
      Node_t *node = new Node_t(Valtype::Object);

      while(LEX_OBJECT_CLOSE != this->lex.cur_sym)
      {
         /* handle empty objects */
         if(LEX_OBJECT_CLOSE == this->lex.next())
            break;

         if(LEX_STRING != this->lex.cur_sym)
            trw_err("Expected node name");

         this->lex.get_str(name);
         if(LEX_STRING != this->lex.cur_sym)
            trw_err("Invalid node name");

         if(LEX_NAME_SEPERATOR != this->lex.next())
            trw_err("Expected name seperator");
         
         this->lex.next();
         Node_t *temp = this->ParseNode(LEX_OBJECT_CLOSE);
         temp->name = name;
         
         NEXT_NEW_NODE(node, curr);
      }

      node->vlast = curr;
      /* move past object close symbol */
      this->lex.next(); 

      return node;
   }

   Parser_t::~Parser_t()
   {
      Helper_t::free_node(this->proot);
   }
}
