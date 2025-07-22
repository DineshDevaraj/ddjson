
#pragma once

#include <iostream>
#include "lexer.h"
#include "doc.h"

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

   struct Parser_t
   {
      Node_t * ParseNode(Lexer_t &lex, Symbol node_close);
      Node_t * ParseObject(Lexer_t &lex);
      Node_t * ParseArray(Lexer_t &lex);
   };
}
