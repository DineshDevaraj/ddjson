
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "doc.h"
#include "node.h"
#include "lexer.h"
#include "parser.h"
#include "helper.h"

namespace ddjson
{
   Doc_t::Doc_t() {
      this->proot = nullptr;
   }

   Node_t & Doc_t::root() { 
      return *this->proot; 
   }

   Node_t & Doc_t::parse_string(const char *json_arg)
   {
      Lexer_t lex;
      Parser_t parser;

      try
      {
         lex.load_string(json_arg);
         if(LEX_OBJECT_OPEN != lex.cur_sym)
            trw_err("Expected object at start");
         this->proot = parser.ParseObject(lex);
         return *this->proot;
      }
      catch(Exception exc)
      {
         error.desc = exc.msg;
         error.line = lex.line;
         error.colum = lex.cur_pos - lex.line_bgn + 1;
         error.offset = lex.cur_pos - lex.json_str + 1;
      }

      return oInvalid;
   }

   Node_t & Doc_t::parse_file(FILE *fh)
   {
      struct stat st;
      char *json_str = NULL;
      fstat(fileno(fh), &st);
      json_str = new char [st.st_size];
      fread(json_str, sizeof(char), st.st_size, fh);
      parse_string(json_str);
      delete [] json_str;
      return *this->proot;
   }

   Node_t & Doc_t::parse_file(const char *file_path)
   {
      FILE *fh = fopen(file_path, "r");
      if(NULL == fh) return oInvalid;
      parse_file(fh);
      fclose(fh);
      return *this->proot;
   }

   Doc_t::~Doc_t()
   {
      Helper_t::free_node(this->proot);
   }
}