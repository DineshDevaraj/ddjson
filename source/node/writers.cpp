
#include "node.h"
#include "helper.h"

namespace ddjson
{
    int Node_t::write(FILE *fh, const char *pad)
   {
      return Helper_t::write(fh, this, pad);
   }
   
   int Node_t::write(char *str, const char *pad)
   {
      return Helper_t::write(str, this, pad);
   }

   int Node_t::write(ostream &os, const char *pad)
   {
      ostream *pos = &os;
      return Helper_t::write(pos, this, pad);
   }
}
