
/*
 *  Copyright [2025] <Dinesh Devaraj>
 */

/*
 *  ⓒ 2025 Dinesh Devaraj - All Rights Reserved
 */

#include <cstdio>
#include <iostream>
#include <string>

#include "header/ddjson.hpp"
#include "header/error.hpp"

void PrintJson(Node_t &node) {
  Iterator_t itr = node.front();
  for (; Node_t &ref = *itr; ++itr) {
    fprintf(stderr, "%s : ", ref.name.data());
    switch (ref.value_type()) {
      case Valtype::Int:
        cerr << (static_cast<int>)ref << endl;
        break;
      case Valtype::Float:
        cerr << (static_cast<float>)ref << endl;
        break;
      case Valtype::String:
        cerr << (static_cast<string>)ref << endl;
        break;
      case Valtype::Array:
      case Valtype::Object:
        fprintf(stderr, "(%d)\n", ref.child_count());
        PrintJson(ref);
        break;
    }
  }
  return;
}

char json_str[5000] = {};

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <start> <end>\n", argv[0]);
    return 1;
  }

  char file[256] = {};
  int start = atoi(argv[1]);
  int end = atoi(argv[2]);
  for (int I = start; I < end; I++) {
    snprintf(file, sizeof(file), "samples/sample%d.json", I);
    FILE *fh = fopen(file, "r");
    if (NULL == fh) {
      printf("Unable to open file %s\n", file);
      break;
    }

    Doc_t oJson;
    Node_t &root = oJson.parse_file(file);
    Error_t &error = oJson.error();
    if (not error.desc.empty()) {
      printf("Error in file %s line %d column %d\n", file, error.line,
             error.colum);
      printf("Error: %s\n\n", error.desc.data());
      continue;
    }

    root.write(json_str, 0);
    printf("%s\n\n", json_str);

    fclose(fh);
  }

  return 0;
}
