
/*
 *  Copyright [2025] <Dinesh Devaraj>
 */

#pragma once

#include <string>

namespace ddjson {
struct Error_t {
  int line;
  int colum;
  int offset;
  std::string desc;
};
}  // namespace ddjson
