
/**
 *
 * Author  : D.Dinesh
 *           www.techybook.com
 *           dinesh@techybook.com
 *
 * Licence : Refer the license file
 *
 **/

#include <cstdio>
#include <iostream>

#include "Icejson.h"

using namespace std;
using namespace Icejson;

void PrintJson(Node_t &node)
{
   Iterator_t itr = node.front();
   for( ; Node_t &ref = *itr; ++itr)
   {
      fprintf(stderr, "%s : ", ref.name.data());
      switch(ref.value_type())
      {
         case Valtype::Int    : cerr << (int)ref << endl;
                                break;
         case Valtype::Char   : cerr << (char)ref << endl;
                                break;
         case Valtype::Float  : cerr << (float)ref << endl;
                                break;
         case Valtype::String : cerr << (string)ref << endl;
                                break;
         case Valtype::Array  :
         case Valtype::Object : fprintf(stderr, "(%d)\n", ref.count());
                                PrintJson(ref);
                                break;
      }
   }
   return;
}

char json_str[5000] = {};

int main()
{
   Doc_t oJson;
   char file[256] = {};

   for(int I = 1; I < 2; I++)
   {
      sprintf(file, "Samples/Sample%d.json", I);
      FILE *fh = fopen(file, "r");
      if(NULL == fh)
      {
         printf("Unable to open file %s\n", file);
         break;
      }

      Node_t &root = oJson.parse_file(file);
      if(!root)
      {
         printf("%s at or before colum %d in line %d\n",
               oJson.error.desc.data(), oJson.error.colum, 
               oJson.error.line);
         break;
      }

      root.write(json_str, "   ");
      puts(json_str);

      fclose(fh);
   }

   return 0;
}
