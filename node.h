
#pragma once

#include <iostream>
#include "forward_declarations.h"

namespace ddjson
{
   using namespace std;

   #define oInvalid (*((Node_t *)0))

   struct Error_t
   {
      int line;
      int colum;
      int offset;
      string desc;
   };
   
   /* different value types supported in JSON */
   struct Valtype
   {
      enum Values
      {
         Int      = 'I',
         Float    = 'F',
         String   = 'S',
         Bool     = 'B',
         Null     = 'N',
         Array    = 'A',
         Object   = 'O',
         Invalid  = 'X'
      };
   };

   typedef Valtype::Values Valtype_t;


   struct MembersMixin_t
   {
      Doc_t *pdoc;

      Node_t *proot;
      Node_t *pnext;
      Node_t *pprev;
      Node_t *pparent;

      int pcount;
      string vstr;
      Valtype_t vtype;

      union
      {
         int vint;
         char vchar;
         bool vbool;
         float vreal;
         struct
         {
            union
            {
               Node_t *varr;
               Node_t *vobj;
            };
            Node_t *vlast;
         };
      };
   };

   struct Node_t : private MembersMixin_t
   {
      Node_t();

      string name;

      Doc_t & doc() const;
      Node_t & root() const;

      Valtype_t value_type() const;

      Node_t & next() const;
      Node_t & prev() const;
      Node_t & parent() const;
      
      int count() const;
      bool valid() const;

      operator bool () const;

      Node_t & operator [] (const int idx) const;
      Node_t & operator [] (const char *name) const;

      bool operator != (const Iterator_t &rhs) const;
      bool operator == (const Iterator_t &rhs) const;

      operator int () const;
      operator char () const;
      operator float () const;
      operator string () const;

      Iterator_t back() const;
      Iterator_t front() const;
      
      int write(FILE *fh, const char *pad = "   ");
      int write(char *fh, const char *pad = "   ");
      int write(ostream &os = cout, const char *pad = "   ");

      friend struct Helper_t;
      friend struct Parser_t;
      friend struct Iterator_t;
   };
}
