
#include "node.h"

namespace ddjson
{
   Node_t::operator int    () const { return this->vint;  }
   Node_t::operator char   () const { return this->vchar; }
   Node_t::operator bool   () const { return this->vbool; }
   Node_t::operator float  () const { return this->vreal; }
   Node_t::operator string () const { return this->vstr;  }

   Node_t & Node_t::operator [] (const int idx) const
   {
      if(Valtype::Array == this->vtype or 
            Valtype::Object == this->vtype)
      {
         Node_t *cur = vobj;
         for(int I = 0; cur && I < idx; I++)
            cur = cur->pnext;
         return *cur;
      }
      return oInvalid;
   }

   Node_t & Node_t::operator [] (const char *name) const
   {
      if(Valtype::Object == this->vtype)
      {
         Node_t *cur = vobj;
         for( ; cur; cur = cur->pnext)
            if(cur->name == name)
               return *cur;
      }
      return oInvalid;
   }
}
