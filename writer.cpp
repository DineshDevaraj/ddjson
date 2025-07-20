
#include <iostream>

#include "node.h"
#include "writer.h"
#include "helper.h"

namespace ddjson
{
   using namespace std;

   Writer_t::Writer_t() 
   {
      int_format = "%d";
      str_format = "%s";
      float_format = "%f";
   }

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