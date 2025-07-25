
/*
 *  Copyright [2025] <Dinesh Devaraj>
 */

#include "header/iterator.hpp"
#include "header/node.hpp"

namespace ddjson {

Node_t& Node_t::root() const { return *this->proot; }

Iterator_t Node_t::front() const {
  if (this != &oInvalid and
      (Valtype::Array == this->vtype or Valtype::Object == this->vtype))
    return Iterator_t(this->vobj);
  return Iterator_t(nullptr);
}

Iterator_t Node_t::back() const {
  if (this != &oInvalid and
      (Valtype::Array == this->vtype or Valtype::Object == this->vtype))
    return Iterator_t(this->vlast);
  return Iterator_t(nullptr);
}

Node_t& Node_t::prev() const { return *this->pprev; }
Node_t& Node_t::next() const { return *this->pnext; }
Node_t& Node_t::parent() const { return *this->pparent; }
}  // namespace ddjson
