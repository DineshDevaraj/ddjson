
/* 
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#include "doc.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.hpp"
#include "node.hpp"
#include "parser.hpp"

namespace ddjson {
Node_t &Doc_t::root() { return this->pparser->root(); }

Error_t &Doc_t::error() { return this->pparser->error; }

Node_t &Doc_t::parse_string(const char *json_str) {
  this->pparser = new Parser_t(json_str);
  this->pparser->ParseRoot();
  return this->root();
}

Node_t &Doc_t::parse_file(FILE *fh) {
  struct stat st;
  char *json_str = NULL;
  fstat(fileno(fh), &st);
  json_str = new char[st.st_size];
  fread(json_str, sizeof(char), st.st_size, fh);
  parse_string(json_str);
  delete[] json_str;
  return this->root();
}

Node_t &Doc_t::parse_file(const char *file_path) {
  FILE *fh = fopen(file_path, "r");
  if (NULL == fh) return oInvalid;
  parse_file(fh);
  fclose(fh);
  return this->root();
}

Doc_t::~Doc_t() { delete this->pparser; }
}  // namespace ddjson
