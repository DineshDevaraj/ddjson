
/* 
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#include "helper.hpp"

#include <cstdarg>
#include <cstdio>
#include <ostream>

namespace ddjson {
template int Helper_t::write<FILE>(FILE *&ptr, Node_t *pn, const char *pad,
                                   int lev);
template int Helper_t::write<char>(char *&ptr, Node_t *pn, const char *pad,
                                   int lev);
template int Helper_t::write<ostream>(ostream *&ptr, Node_t *pn,
                                      const char *pad, int lev);

Node_t *Helper_t::free_node(Node_t *pnode) {
  if (nullptr == pnode) return nullptr;

  Node_t *next = pnode->vobj;

  if (Valtype::Array == pnode->vtype or Valtype::Object == pnode->vtype)
    while (next = Helper_t::free_node(next));

  next = pnode->pnext;
  delete pnode;
  return next;
}

template <>
int Helper_t::print(FILE *&fh, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  return vfprintf(fh, fmt, args);
}

template <>
int Helper_t::print(char *&cp, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len = vsprintf(cp, fmt, args);
  cp += len;
  return len;
}

template <>
int Helper_t::print(ostream *&os, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(0, 0, fmt, args);
  if (len > 0) {
    string str;
    str.resize(len);
    vsprintf(&str[0], fmt, args);
    (*os) << str;
  }
  return len;
}

template <typename tn> /* tn - template name */
int Helper_t::write(tn *&dst, Node_t *pnode, const char *pad, int lev) {
  int len = 0;
  Node_t *itr = NULL;

  if (pad)
    for (int I = 0; I < lev; I++) len += print(dst, "%s", pad);

  if (not pnode->name.empty()) {
    len += print(dst, "\"%s\"", pnode->name.data());
    if (pad)
      print(dst, " : ");
    else
      print(dst, ":");
  }

  switch (pnode->vtype) {
    case Valtype::Int:
      len += print(dst, "%d", pnode->vint);
      break;

    case Valtype::Bool:
      len += print(dst, "%s", pnode->vbool ? "true" : "false");
      break;

    case Valtype::Float:
      len += print(dst, "%f", pnode->vreal);
      break;

    case Valtype::String:
      len += print(dst, R"("%s")", pnode->vstr.data());
      break;

    case Valtype::Array:
      len += print(dst, "[");
      if (pnode->vobj) {
        if (pad) print(dst, "\n");
        for (itr = pnode->vobj; itr;) {
          len += write(dst, itr, pad, lev + 1);
          itr = itr->pnext;
          if (itr) len += print(dst, ",");
          if (pad) len += print(dst, "\n");
        }
        if (pad)
          for (int I = 0; I < lev; I++) len += print(dst, "%s", pad);
      }
      len += print(dst, "]");
      break;

    case Valtype::Object:
      len += print(dst, "{");
      if (pnode->vobj) {
        if (pad) print(dst, "\n");
        for (itr = pnode->vobj; itr;) {
          len += write(dst, itr, pad, lev + 1);
          itr = itr->pnext;
          if (itr) len += print(dst, ",");
          if (pad) len += print(dst, "\n");
        }
        if (pad)
          for (int I = 0; I < lev; I++) len += print(dst, "%s", pad);
      }
      len += print(dst, "}");
      break;

    case Valtype::Null:
      len += print(dst, "null");
      break;
  }

  return len;
}
}  // namespace ddjson