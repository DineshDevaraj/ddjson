
/**
 *
 * Author  : D.Dinesh
 *           www.techybook.com
 *           dinesh@techybook.com
 *
 * Licence : Refer the license file
 *
 **/

#pragma once

#include <iostream>

namespace Icejson
{
   using namespace std;

   /* forward declarations */
   struct Doc_t;
   struct Node_t;
   struct Error_t;
   struct Parser_t;
   struct Iterator_t;

   /* different value types supported in JSON */
   struct Valtype
   {
      enum Values
      {
         Int      = 'I',
         Float    = 'F',
         Char     = 'C',
         String   = 'S',
         Bool     = 'B',
         Null     = 'N',
         Array    = 'A',
         Object   = 'O',
         Invalid  = 'X'
      };
   };

   typedef Valtype::Values Valtype_t;

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

      Node_t & root();

      Node_t & parse_file(FILE *);
      Node_t & parse_file(const char *);
      Node_t & parse_string(const char *);

      private : Node_t *proot;
   };

   struct Members_t
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

   struct Node_t : private Members_t
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

      friend struct Parser_t;
      friend struct Iterator_t;
   };

   struct Iterator_t
   {
      Iterator_t();
      Iterator_t(Node_t *node);

      operator bool ();
      Node_t & operator * ();

      Iterator_t & operator ++();   /* prefix */
      Iterator_t & operator --();   /* prefix */

      Iterator_t operator ++(int);  /* postfix */
      Iterator_t operator --(int);  /* postfix */

      bool operator != (const Iterator_t &rhs);
      bool operator == (const Iterator_t &rhs);

      private : Node_t *pcur;
   };
}
