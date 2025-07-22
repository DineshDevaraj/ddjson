
#include "iterator.h"

namespace ddjson
{
   Iterator_t::Iterator_t() {}

   Iterator_t::Iterator_t(Node_t *pnode) :
      pcurr(pnode) {}

   Iterator_t::operator bool()
   {
      return this->pcurr;
   }

   Node_t & Iterator_t::operator * ()
   {
      return *this->pcurr;
   }
   
   Iterator_t & Iterator_t::operator ++ ()
   {
      this->pcurr = this->pcurr->pnext;
      return *this;
   }

   Iterator_t & Iterator_t::operator -- ()
   {
      this->pcurr = this->pcurr->pprev;
      return *this;
   }

   Iterator_t Iterator_t::operator ++ (int)
   {
      Iterator_t itr(this->pcurr);
      this->pcurr = this->pcurr->pnext;
      return itr;
   }

   Iterator_t Iterator_t::operator -- (int)
   {
      Iterator_t itr(this->pcurr);
      this->pcurr = this->pcurr->pprev;
      return itr;
   }

   bool Iterator_t::operator == (const Iterator_t &rhs)
   {
      return this->pcurr == rhs.pcurr;
   }

   bool Iterator_t::operator != (const Iterator_t &rhs)
   {
      return this->pcurr != rhs.pcurr;
   }
}