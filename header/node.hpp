
/* 
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#pragma once

#include <iostream>

#include "forward_declarations.hpp"

namespace ddjson {
using namespace std;

#define oInvalid (*((Node_t *)0))

/* different value types supported in JSON */
struct Valtype {
  enum Values {
    Int = 'I',
    Float = 'F',
    String = 'S',
    Bool = 'B',
    Null = 'N',
    Array = 'A',
    Object = 'O',
    Invalid = 'X'
  };
};

typedef Valtype::Values Valtype_t;

struct MembersMixin_t {
  Node_t *proot;
  Node_t *pnext;
  Node_t *pprev;
  Node_t *pparent;

  int m_child_count;

  std::string vstr;
  Valtype_t vtype;

  union {
    int vint;
    char vchar;
    bool vbool;
    float vreal;
    struct {
      union {
        Node_t *varr;
        Node_t *vobj;
      };
      Node_t *vlast;
    };
  };
};

struct Node_t : private MembersMixin_t {
  string name;

  Node_t();
  Node_t(Valtype_t type);

  Node_t &root() const;

  Valtype_t value_type() const;

  Node_t &next() const;
  Node_t &prev() const;
  Node_t &parent() const;

  bool is_valid() const;
  int child_count() const;

  operator int() const;
  operator bool() const;
  operator char() const;
  operator float() const;
  operator string() const;

  Node_t &operator[](const int idx) const;
  Node_t &operator[](const char *name) const;

  Iterator_t back() const;
  Iterator_t front() const;

  int write(FILE *fh, const char *pad = "   ");
  int write(char *fh, const char *pad = "   ");
  int write(ostream &os = cout, const char *pad = "   ");

  friend struct Helper_t;
  friend struct Parser_t;
  friend struct Iterator_t;
};
}  // namespace ddjson
