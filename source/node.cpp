
/*
 *  Copyright [2025] <Dinesh Devaraj>
 */

#include "header/node.hpp"

#include <cstdio>
#include <iostream>

#include "header/helper.hpp"
#include "header/iterator.hpp"

namespace ddjson {
Node_t::Node_t() {
  this->vobj = nullptr;
  this->vlast = nullptr;

  this->m_child_count = 0;

  this->proot = nullptr;
  this->pnext = nullptr;
  this->pprev = nullptr;
  this->pparent = nullptr;

  this->vtype = Valtype::Invalid;
}

Node_t::Node_t(Valtype_t type) : Node_t() { this->vtype = type; }

bool Node_t::is_valid() const { return this != &oInvalid; }

int Node_t::child_count() const { return this->m_child_count; }

Valtype_t Node_t::value_type() const { return this->vtype; }
}  // namespace ddjson
