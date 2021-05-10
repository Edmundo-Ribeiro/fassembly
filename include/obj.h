#ifndef OBJ_H
#define OBJ_H
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

typedef multimap<string, int> USE_TABLE;
typedef map<string, int> SYMBOLS_TABLE;
typedef vector<int> code_data;
class OBJ {
  string name;
  string bitmap;
  int size;
  code_data data;
  USE_TABLE use_table;
  SYMBOLS_TABLE def_table;  //definition table

 public:
  SYMBOLS_TABLE deserialize_st(string table) {
    stringstream source(table);
    string buffer;
    string key;
    SYMBOLS_TABLE aux;
    int value;
    int len;
    getline(source, buffer, ' ');
    len = stoi(buffer);

    for (int i = 0; i < len; ++i) {
      getline(source, key, ' ');
      getline(source, buffer, ' ');

      value = stoi(buffer);
      aux[key] = value;
    }

    return aux;
  }

  USE_TABLE deserialize_ut(string table) {
    stringstream source(table);
    string buffer;
    string key;
    USE_TABLE aux;
    int value;
    int len;
    getline(source, buffer, ' ');
    len = stoi(buffer);

    for (int i = 0; i < len; ++i) {
      getline(source, key, ' ');
      getline(source, buffer, ' ');
      value = stoi(buffer);
      aux.insert(pair<string, int>(key, value));
    }
    return aux;
  }

  code_data deserialize_dt(string source) {
    stringstream ss(source);
    code_data _data;
    for (string buffer; getline(ss, buffer, ' '); _data.push_back(stoi(buffer))) {
    }
    return _data;
  }

  OBJ(string name, int size, string bitmap, SYMBOLS_TABLE def_table, USE_TABLE use_table, code_data data) {
    this->name = name;
    this->size = size;
    this->bitmap = bitmap;
    this->def_table = def_table;
    this->use_table = use_table;
    this->data = data;
  }
  OBJ() {
  }
  ~OBJ() {
  }
  bool read_file(string filename) {
    OBJ return_obj;
    string buffer;
    ifstream source = ifstream(filename);

    if (!source.is_open()) {
      return false;
    }
    getline(source, name);

    getline(source, buffer);
    size = stoi(buffer);
    getline(source, bitmap);

    getline(source, buffer);
    def_table = deserialize_st(buffer);

    getline(source, buffer);
    use_table = deserialize_ut(buffer);

    getline(source, buffer);
    data = deserialize_dt(buffer);

    return true;
  }

  void print() {
    cout << "Name: " << name << " Size: " << size << endl;
    cout << "Bitmap: " << bitmap << endl;
    for (auto &x : def_table) {
      cout << "[" << x.first << "] : [" << x.second << "]" << endl;
    }
    cout << endl;

    for (auto &x : use_table) {
      cout << "[" << x.first << "] : [" << x.second << "]" << endl;
    }
    cout << endl;

    for (auto it = data.begin(); it != data.end(); ++it) {
      cout << *it << " ";
    }
    cout << endl;
  }
};

#endif