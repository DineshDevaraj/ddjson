
/* 
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#pragma once

#include <iostream>

#include "error.hpp"
#include "lexer.hpp"
#include "node.hpp"

namespace ddjson {
#define OK true
#define ERR false

#define NEXT_NEW_NODE(node, curr) \
  ({                              \
    temp->pprev = curr;           \
    if (curr == nullptr)          \
      node->vobj = temp;          \
    else                          \
      curr->pnext = temp;         \
    node->m_child_count++;        \
    curr = temp;                  \
  })

struct Parser_t {
  Parser_t(const char *json_str);

 public:
  Node_t &root();
  Node_t *ParseRoot();

 private:
  Node_t *ParseNode(Symbols node_close);
  Node_t *ParseNumbers();
  Node_t *ParseString();
  Node_t *ParseBool(bool val);
  Node_t *ParseObject();
  Node_t *ParseArray();
  Node_t *ParseNull();

 public:
  Error_t error;

 private:
  Node_t *proot = nullptr;
  Lexer_t lex;

 public:
  ~Parser_t();
};
}  // namespace ddjson
