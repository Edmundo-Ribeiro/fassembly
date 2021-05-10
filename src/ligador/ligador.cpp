#include <fstream>
#include <iostream>

#include "../../include/obj.h"
using namespace std;

void print_table(SYMBOLS_TABLE &st) {
  for (auto &x : st) {
    std::cout << "[" << x.first << "] : [" << x.second << "]" << endl;
  }
  std::cout << endl;
}

int main(int argc, char **argv) {
  int number_of_files = argc - 1;
  string fname;
  ifstream source;
  SYMBOLS_TABLE global_def_table;
  vector<OBJ> objs;
  ofstream output;
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
    objs.push_back(obj);
  }

  int acc = 0;

  //criar tabela de definição global
  for (auto &o : objs) {
    o.set_correction_factor(acc);

    for (auto &entry : o.get_def_table()) {
      global_def_table[entry.first] = entry.second + o.get_factor();
    }
    acc += o.get_size();
  }

  for (auto &o : objs) {
    code_data obj_data = o.get_data();
    string bitmap = o.get_bitmap();
    int factor = o.get_factor();
    for (auto &entry : o.get_use_table()) {
      // cout << "obj_data.at(" << entry.second << ") = global_def_table[" << entry.first << "] = " << global_def_table[entry.first] << endl;
      obj_data.at(entry.second) = global_def_table[entry.first];
    }

    for (int i = 0; i < o.get_size(); ++i) {
      obj_data.at(i) += (bitmap[i] == '1' ? factor : 0);
    }

    o.set_data(obj_data);
  }

  output = ofstream(objs.at(0).get_name() + "_ligado.obj");
  for (auto &o : objs) {
    output << o.get_serialized_data();
    o.print();
  }

  print_table(global_def_table);
  output.close();
  return 0;
}