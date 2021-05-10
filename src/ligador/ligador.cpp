#include <fstream>
#include <iostream>

#include "../../include/obj.h"
using namespace std;

int main(int argc, char **argv) {
  int number_of_files = argc - 1;
  string fname;
  ifstream source;
  OBJ obj;
  for (uint8_t i = 0; i < number_of_files; ++i) {
    fname = string(argv[i + 1] != NULL ? argv[i + 1] : "");
    source = ifstream(fname);

    if (fname == "") {
      std::cout << "É necessário informar um arquivo .asm" << endl;
      return 1;
    }

    if (!obj.read_file(fname)) {
      std::cout << "Erro ao tentar abrir: " << fname << endl;
      return 1;
    }

    obj.print();
  }

  return 0;
}