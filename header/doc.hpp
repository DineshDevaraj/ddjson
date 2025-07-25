
/* 
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#pragma once

#include "node.hpp"
#include "parser.hpp"

namespace ddjson {
struct Doc_t {
  Node_t &root();
  Error_t &error();

  Node_t &parse_file(FILE *);
  Node_t &parse_file(const char *);
  Node_t &parse_string(const char *);

  ~Doc_t();

 private:
  Parser_t *pparser = nullptr;
};
}  // namespace ddjson
