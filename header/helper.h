
#pragma once

#include "node.h"

namespace ddjson
{
   struct Helper_t
   {
      static void free_node(Node_t * &pnode);

      template <typename tn>
      static int print(tn * &ptr, const char *fmt, ...);

      template <typename tn>
      static int write(tn * &ptr, Node_t *pn, const char *pad = "\0", int lev = 0);
   };
}
