
#include "node.h"
#include "helper.h"
#include "iterator.h"

namespace ddjson
{
   Node_t::Node_t()    
   {
      this->pdoc = nullptr;
      this->vobj = nullptr;
      this->vlast = nullptr;

      this->child_count = 0;

      this->proot = nullptr;
      this->pnext = nullptr;
      this->pprev = nullptr;
      this->pparent = nullptr;

      this->vtype = Valtype::Invalid;
   }

   Node_t::Node_t(Valtype_t type) : Node_t()
   {
      this->vtype = type;
   }

   Doc_t & Node_t::doc() const { return *this->pdoc; }

   int Node_t::count() const { return this->child_count; }

   bool Node_t::valid() const { return this != &oInvalid; }

   Valtype_t Node_t::value_type() const { return this->vtype; }
}
