
/**
 *
 * Author   : D.Dinesh
 * Website  : www.techybook.com
 * Email Id : dinesh@techybook.com
 *
 * Created  : 24 Feb 2015 - Tue
 * Updated  : 25 Feb 2015 - Wed
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
   struct Valuetype
   {
      enum Values
      {
         Int      = 'I',
         Float    = 'F',
         Char     = 'C',
         String   = 'S',
         Array    = 'A',
         Object   = 'O',
         Invalid  = 'X'
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
      Doc_t(Node_t *node);

      Error_t error;

      Node_t & get_root();

      Node_t & parse_file(FILE *);
      Node_t & parse_file(const char *);
      Node_t & parse_string(const char *);

      private : Node_t *proot;
   };

   struct Node_t
   {
      Node_t();

      string name;

      Doc_t & get_doc();
      Node_t & get_root();

      Valuetype_t get_value_type();

      template <typename type> type value();

      Node_t & next();
      Node_t & prev();

      bool valid();

      operator bool ();

      bool operator != (const Iterator_t &rhs);
      bool operator == (const Iterator_t &rhs);

      Iterator_t begin();
      Iterator_t end();

      private :

      Doc_t *pdoc;

      Node_t *proot;
      Node_t *pnext;
      Node_t *pprev;
      Node_t *pparent;

      string vstr;

      union
      {
         int vint;
         float vreal;
         char vchar;
         Node_t *varr;
         Node_t *vobj;
      };

      Valuetype_t vtype;

      friend struct Parser_t;
      friend struct Iterator_t;
   };

   struct Iterator_t
   {
      Iterator_t();
      Iterator_t(Node_t *node);

      Node_t & operator * ();

      Iterator_t & operator ++();      /* prefix */
      Iterator_t & operator --();      /* prefix */

      Iterator_t operator ++(int);   /* postfix */
      Iterator_t operator --(int);   /* postfix */

      operator bool ();
      bool operator not ();

      bool operator != (const Iterator_t &rhs);
      bool operator == (const Iterator_t &rhs);

      private : Node_t *pcur;
   };
}