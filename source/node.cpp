
#include "node.h"
#include "helper.h"
#include "iterator.h"

namespace ddjson
{
   Node_t::Node_t()    
   {
      pdoc = nullptr;
      vobj = nullptr;

      proot = nullptr;
      pnext = nullptr;
      pprev = nullptr;
      pparent = nullptr;
   
      child_count = 0;

      vtype = Valtype::Invalid;
   }

   Doc_t & Node_t::doc() const { return *this->pdoc; }

   Valtype_t Node_t::value_type() const { return vtype; }

   bool Node_t::valid() const { return this != &oInvalid; }

   int Node_t::count() const { return this->child_count; }
}
