
#pragma once

#include <iostream>
#include "lexer.h"
#include "node.h"
#include "doc.h"

namespace ddjson
{
   #define OK   true
   #define ERR  false

   #define NEXT_NEW_NODE(ptr) \
   ({ \
         Parser_t *swp = new Parser_t(pdoc); \
         swp->pprev = ptr; \
         ptr->pnext = swp; \
         ptr->pparent = this; \
         ptr = swp; \
    })

   struct Parser_t : public Node_t
   {
      Parser_t(Doc_t *doc) { pdoc = doc; }

      Parser_t(Doc_t *doc, Valtype_t type) 
      { 
         pdoc = doc;
         vtype = type; 
      }

      bool ParseArray(Lexer_t &lex);
      bool ParseObject(Lexer_t &lex);
      bool ParseNode(Lexer_t &lex, Symbol node_close);
   };
}
