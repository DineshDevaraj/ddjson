
#pragma once

#include <iostream>
#include "lexer.h"
#include "error.h"
#include "node.h"

namespace ddjson
{
   #define OK   true
   #define ERR  false

   #define NEXT_NEW_NODE(node, curr)            \
   ({                                           \
         temp->pprev = curr;                    \
         if(curr == nullptr) node->vobj = temp; \
         else curr->pnext = temp;               \
         node->child_count++;                   \
         curr = temp;                           \
   })

   struct Error_t
   {
      int line;
      int colum;
      int offset;
      string desc;
   };

   struct Parser_t
   {
      Parser_t(const char *json_str);

      public:
         Node_t & root();
         Node_t * ParseRoot();

      private:
         Node_t * ParseNode(Symbol node_close);
         Node_t * ParseNumbers();
         Node_t * ParseString();
         Node_t * ParseBool(bool val);
         Node_t * ParseObject();
         Node_t * ParseArray();
         Node_t * ParseNull();

      public:
         Error_t error;

      private:
         Node_t *proot = nullptr;
         Lexer_t lex;

      public:
         ~Parser_t();
   };
}
