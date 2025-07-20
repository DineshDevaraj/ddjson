
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
        (Valtype::Array == vtype or
            Valtype::Object == vtype))
         return Iterator_t(vobj);
      return Iterator_t(NULL);
   }

   Iterator_t Node_t::back() const
   {
      if(this != &oInvalid and
        (Valtype::Array == vtype or
            Valtype::Object == vtype))
         return Iterator_t(vlast);
      return Iterator_t(NULL);
   }

   Doc_t & Node_t::doc() const     { return *pdoc;    }

   Node_t & Node_t::root() const   { return *proot;   }
   Node_t & Node_t::prev() const   { return *pprev;   }
   Node_t & Node_t::next() const   { return *pnext;   }
   Node_t & Node_t::parent() const { return *pparent; }

   int Node_t::count() const       { return pcount;   }

   bool Node_t::valid() const     { return this != &oInvalid; }
   Node_t::operator bool () const { return this != &oInvalid; }

   Valtype_t Node_t::value_type() const { return vtype; }

   Node_t::operator int    () const { return vint;  }
   Node_t::operator char   () const { return vchar; }
   Node_t::operator float  () const { return vreal; }
   Node_t::operator string () const { return vstr;  }

   Node_t & Node_t::operator [] (const int idx) const
   {
      if(Valtype::Array == vtype or 
            Valtype::Object == vtype)
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
      if(Valtype::Object == vtype)
      {
         Node_t *cur = vobj;
         for( ; cur; cur = cur->pnext)
            if(cur->name == name)
               return *cur;
      }
      return oInvalid;
   }
}