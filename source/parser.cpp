
/*
 *  Copyright [2025] <Dinesh Devaraj>
 */

#include "header/parser.hpp"

#include <string>

#include "header/helper.hpp"

namespace ddjson {
Parser_t::Parser_t(const char *json_str) { this->lex.load_string(json_str); }

Node_t *Parser_t::ParseRoot() {
  try {
    if (Symbols::ObjectOpen != this->lex.cur_sym)
      throw_ddjex("Expected object-open `{` as first character");
    this->proot = this->ParseObject();
    return this->proot;
  } catch (DDJException exc) {
    error.desc = exc.desc;
    error.line = this->lex.line;
    error.colum = this->lex.cur_pos - this->lex.line_bgn + 1;
    error.offset = this->lex.cur_pos - this->lex.json_str + 1;
  }
  return nullptr;
}

Node_t &Parser_t::root() { return *this->proot; }

/**
 * s2s - symbol to string
 */
string s2s(Symbols sym) {
  if (sym == Symbols::Invalid) return "null";
  return string("") + (static_cast<char>)sym;
}

Node_t *Parser_t::ParseNode(Symbols node_close) {
  Node_t *node = nullptr;

  switch (this->lex.cur_sym) {
    case Symbols::Neg:
    case Symbols::Int:
      node = this->ParseNumbers();
      break;
    case Symbols::DoubleQuote:
      node = this->ParseString();
      break;
    case Symbols::True:
      node = this->ParseBool(true);
      break;
    case Symbols::False:
      node = this->ParseBool(false);
      break;
    case Symbols::Null:
      node = this->ParseNull();
      break;
    case Symbols::ArrayOpen:
      node = this->ParseArray();
      break;
    case Symbols::ObjectOpen:
      node = this->ParseObject();
      break;

    default:
      std::string msg;
      std::string symbol = s2s(this->lex.cur_sym);
      msg = R"(Expected number, char, double-quote `"`,)";
      msg += " array-open `[` or object-open `{`";
      msg += " but got `" + symbol + "` instead";
      throw_ddjex(msg.data());
  }

  if (Symbols::Comma != this->lex.cur_sym and node_close != this->lex.cur_sym) {
    std::string close_symbol = s2s(node_close);
    std::string curr_symbol = s2s(this->lex.cur_sym);
    std::string msg = "Expected colon `:`, comma `,`";
    msg += " or close-symbol `" + close_symbol + "` but got `";
    msg += curr_symbol + "` instead";
    throw_ddjex(msg.data());
  }

  return node;
}

Node_t *Parser_t::ParseNumbers() {
  const char *val;
  Node_t *node = new Node_t();

  if (Symbols::Int == this->lex.get_num(val)) {
    node->vtype = Valtype::Int;
    node->vint = atoi(val);
  } else {
    node->vtype = Valtype::Float;
    node->vreal = atof(val);
  }

  return node;
}

Node_t *Parser_t::ParseString() {
  Node_t *node = new Node_t(Valtype::String);
  this->lex.get_str(node->vstr);
  if (Symbols::DoubleQuote != this->lex.cur_sym)
    throw_ddjex("Unterminated string value");
  this->lex.next();
  return node;
}

Node_t *Parser_t::ParseBool(bool val) {
  Node_t *node = new Node_t(Valtype::Bool);
  node->vbool = val;
  this->lex.next();
  return node;
}

Node_t *Parser_t::ParseNull() {
  Node_t *node = new Node_t(Valtype::Null);
  this->lex.next();
  return node;
}

Node_t *Parser_t::ParseArray() {
  Node_t *node = new Node_t(Valtype::Array);

  /* move past the array open symbol */
  this->lex.next();

  /* handle empty array */
  if (Symbols::ArrayClose != this->lex.cur_sym) {
    Node_t *curr;
    for (curr = nullptr;; this->lex.next()) {
      Node_t *temp = this->ParseNode(Symbols::ArrayClose);
      NEXT_NEW_NODE(node, curr);
      if (Symbols::ArrayClose == this->lex.cur_sym) break;
    }
    node->vlast = curr;
  }

  /* move past array close symbol */
  this->lex.next();

  return node;
}

Node_t *Parser_t::ParseObject() {
  string name;
  Node_t *curr = nullptr;
  Node_t *node = new Node_t(Valtype::Object);

  while (Symbols::ObjectClose != this->lex.cur_sym) {
    /* handle empty objects */
    if (Symbols::ObjectClose == this->lex.next()) break;

    if (Symbols::DoubleQuote != this->lex.cur_sym)
      throw_ddjex("Expected node name");

    this->lex.get_str(name);
    if (Symbols::DoubleQuote != this->lex.cur_sym)
      throw_ddjex("Invalid node name");

    if (Symbols::Colon != this->lex.next()) throw_ddjex("Expected colon `:`");

    this->lex.next();
    Node_t *temp = this->ParseNode(Symbols::ObjectClose);
    temp->name = name;

    NEXT_NEW_NODE(node, curr);
  }

  node->vlast = curr;
  /* move past object close symbol */
  this->lex.next();

  return node;
}

Parser_t::~Parser_t() { Helper_t::free_node(this->proot); }
}  // namespace ddjson
