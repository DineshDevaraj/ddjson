
/*
 *  Copyright [2025] <Dinesh Devaraj>
 */

#pragma once

#include "header/forward_declarations.hpp"

namespace ddjson {
struct Iterator_t {
  Iterator_t();
  explicit Iterator_t(Node_t *node);

  operator bool();
  Node_t &operator*();

  Iterator_t &operator++(); /* prefix */
  Iterator_t &operator--(); /* prefix */

  Iterator_t operator++(int); /* postfix */
  Iterator_t operator--(int); /* postfix */

  bool operator!=(const Iterator_t &rhs);
  bool operator==(const Iterator_t &rhs);

 private:
  Node_t *pcurr;
};
}  // namespace ddjson
