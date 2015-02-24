
/**
 *
 * Author   : D.Dinesh
 * Website  : www.techybook.com
 * Email Id : dinesh@techybook.com
 *
 * Created  : 24 Feb 2015 - Tue
 * Updated  : 24 Feb 2015 - Tue
 *
 * Licence : Refer the license file
 *
 **/
 
#pragma once

#include <iostream>

using namespace std;

#define T template typename<Type>

struct Valuetype
{
   enum Values
   {
      Int,
      Float,
      Char,
      String,
      Array,
      Object,
      Invalid
   };
};

typedef Valuetype::Values Valuetype_t;

struct Error_t
{
   int line;
   int colum;
   int offset;
   string desc;
};

struct Doc_t
{
   Doc_t();
   
   Error_t error;
   
   Node_t & get_root();
   
   Node_t & parse_file(FILE *);
   Node_t & parse_file(const char *);
   Node_t & parse_string(const char *);
   
   private :
      Node_t *root;
};

struct Node_t
{
   Node_t();

   Doc_t & get_doc();
   Node_t & get_root();

   Valuetype_t get_value_type();

   T int value<int>();
   
   T char value<char>();
   
   T float value<float>();
   
   T string value<string>();
   
   T Node_t & value<Node_t>();

   Node_t & next();
   Node_t & prev();
   
   private :
   
      Doc_t *doc;
      
      Node_t *root;
      Node_t *next;
      Node_t *prev;
      Node_t *parent;
      
      union
      {
         int vint;
         float vreal;
         char vchar;
         string vstr;
         Node_t *varr;
         Node_t *vobj;
      };
      
      Valuetype_t vtype;   
};