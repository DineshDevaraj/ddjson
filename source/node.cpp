
#include "node.h"
#include "helper.h"
#include "iterator.h"

namespace ddjson
{
   Node_t::Node_t()    
   {
      pcount = 0;

      pdoc = nullptr;
      vobj = nullptr;

      proot = nullptr;
      pnext = nullptr;
      pprev = nullptr;
      pparent = nullptr;

      vtype = Valtype::Invalid;
   }

   Doc_t & Node_t::doc() const { return *this->pdoc; }

   Valtype_t Node_t::value_type() const { return vtype; }

   bool Node_t::valid() const { return this != &oInvalid; }

   int Node_t::count() const { return this->pcount; }
}
