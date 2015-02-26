
/**
 *
 * Author   : D.Dinesh
 * Website  : www.techybook.com
 * Email Id : dinesh@techybook.com
 *
 * Created  : 25 Feb 2015 - Wed
 * Updated  : 26 Feb 2015 - Thu
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
   Iterator_t itr = node.begin();
   for( ; itr != node.end(); itr++)
   {
      printf("%s : ", (*itr).name.data());
      switch((*itr).get_value_type())
      {
         case Valuetype::Int    : printf("%d\n", (*itr).value<int>());
                                  break;
         case Valuetype::Float  : printf("%f\n", (*itr).value<float>());
                                  break;
         case Valuetype::Char   : printf("%c\n", (*itr).value<char>());
                                  break;
         case Valuetype::String : printf("%s\n", (*itr).value<string>().data());
                                  break;
         case Valuetype::Array  :
         case Valuetype::Object : printf("\n");
                                  PrintJson((*itr).value<Icejson::Node_t &>());
                                  break;
         default : printf("\n");
      }
   }
   return;
}

char json_str[5000] = {};

int main()
{
   Doc_t oJson;

   FILE *fh = fopen("Sample.json", "r");
   fread(json_str, sizeof(char), sizeof json_str, fh);

   Node_t &root = oJson.parse_string(json_str);
   if(!root)
   {
      printf("%s before line %d colum %d and offset %d\n",
            oJson.error.desc.data(), oJson.error.line, 
            oJson.error.colum, oJson.error.offset);
   }

   PrintJson(root);

   return 0;
}
