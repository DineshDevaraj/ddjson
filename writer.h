
#pragma once

#include <iostream>

namespace ddjson
{
   using namespace std;

   struct Writer_t
   {
      Writer_t();
      string int_format;
      string str_format;
      string float_format;
   };
}
