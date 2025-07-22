
#pragma once

#include <iostream>
#include "lexer.h"
#include "doc.h"

namespace ddjson
{
   #define OK   true
   #define ERR  false

   #define NEXT_NEW_NODE(curr) \
   ({ \
         Node_t swp = new Node_t(); \
         swp.pprev = &curr; \
         curr.pnext = &swp; \
         curr.pparent = &this; \
         curr = swp; \
    })

   struct Parser_t
   {
      Node_t * ParseNode(Lexer_t &lex, Symbol node_close);
      Node_t * ParseObject(Lexer_t &lex);
      Node_t * ParseArray(Lexer_t &lex);
   };
}
