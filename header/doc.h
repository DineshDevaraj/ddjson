
#pragma once

#include "forward_declarations.h"
#include "node.h"

namespace ddjson
{
   struct Doc_t
   {
      Doc_t();

      Error_t error;

      Node_t & root();

      Node_t & parse_file(FILE *);
      Node_t & parse_file(const char *);
      Node_t & parse_string(const char *);

      ~Doc_t();

      private : Node_t *proot;
   };
}
