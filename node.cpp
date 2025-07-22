
#include "node.h"
#include "iterator.h"

namespace ddjson
{
   Node_t::Node_t()    
   {
      pcount = 0;

      pdoc = NULL;
      vobj = NULL;

      proot = NULL;
      pnext = NULL;
      pprev = NULL;
      pparent = NULL;

      vtype = Valtype::Invalid;
   }

   Iterator_t Node_t::front() const
   {
      if(this != &oInvalid and
        (Valtype::Array == this->vtype or
            Valtype::Object == this->vtype))
         return Iterator_t(this->vobj);
      return Iterator_t(NULL);
   }

   Iterator_t Node_t::back() const
   {
      if(this != &oInvalid and
        (Valtype::Array == this->vtype or
            Valtype::Object == this->vtype))
         return Iterator_t(this->vlast);
      return Iterator_t(NULL);
   }

   Doc_t & Node_t::doc() const     { return *this->pdoc;    }

   Node_t & Node_t::root() const   { return *this->proot;   }
   Node_t & Node_t::prev() const   { return *this->pprev;   }
   Node_t & Node_t::next() const   { return *this->pnext;   }
   Node_t & Node_t::parent() const { return *this->pparent; }

   int Node_t::count() const       { return this->pcount;   }

   bool Node_t::valid() const     { return this != &oInvalid; }
   Node_t::operator bool () const { return this != &oInvalid; }

   Valtype_t Node_t::value_type() const { return vtype; }

   Node_t::operator int    () const { return this->vint;  }
   Node_t::operator char   () const { return this->vchar; }
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