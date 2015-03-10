
/**
 *
 * Author  : D.Dinesh
 *           www.techybook.com
 *           dinesh@techybook.com
 *
 * Licence : Refer the license file
 *
 **/

#include <stdio.h>
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
         case Valuetype::Int    : cerr << (int)ref << endl;
                                  break;
         case Valuetype::Char   : cerr << (char)ref << endl;
                                  break;
         case Valuetype::Float  : cerr << (float)ref << endl;
                                  break;
         case Valuetype::String : cerr << (string)ref << endl;
                                  break;
         case Valuetype::Array  :
         case Valuetype::Object : fprintf(stderr, "(%d)\n", ref.count());
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
      fread(json_str, sizeof(char), sizeof json_str, fh);

      Node_t &root = oJson.parse_string(json_str);
      if(!root)
      {
         printf("%s at or before colum %d in line %d\n",
               oJson.error.desc.data(), oJson.error.colum, 
               oJson.error.line);
         break;
      }

      PrintJson(root);

      fclose(fh);
   }

   return 0;
}
