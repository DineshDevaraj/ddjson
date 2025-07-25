
/* 
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#pragma once
#include <iostream>
namespace ddjson {
struct Error_t {
  int line;
  int colum;
  int offset;
  std::string desc;
};
}  // namespace ddjson
