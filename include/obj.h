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
  int correction_factor = 0;

 public:
  int get_size() { return size; }
  string get_bitmap() { return bitmap; }
  code_data get_data() { return data; }
  USE_TABLE get_use_table() { return use_table; }
  SYMBOLS_TABLE get_def_table() { return def_table; }
  string get_name() { return name; }
  int get_factor() { return correction_factor; }

  void set_correction_factor(int factor) {
    this->correction_factor = factor;
  }

  void set_data(code_data &_data) {
    this->data = _data;
  }
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

  void flip_bit_bitmap(int pos) {
    bitmap[pos] = bitmap[pos] == '0' ? '1' : '0';
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

    source.close();
    return true;
  }

  string get_serialized_data() {
    stringstream out;
    for (auto it = data.begin(); it != data.end(); ++it) {
      out << *it << " ";
    }

    return out.str();
  }
  void print() {
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Name: " << name << " | Size: " << size << " | Factor: " << correction_factor << endl;
    cout << "Bitmap: " << bitmap << endl;
    cout << "Definition table:" << endl;
    for (auto &x : def_table) {
      cout << "[" << x.first << "] : [" << x.second << "]" << endl;
    }
    cout << endl;

    cout << "Use table:" << endl;
    for (auto &x : use_table) {
      cout << "[" << x.first << "] : [" << x.second << "]" << endl;
    }
    cout << endl;
    cout << "Data:" << endl;

    for (auto it = data.begin(); it != data.end(); ++it) {
      cout << *it << " ";
    }
    cout << endl;

    cout << "--------------------------------------------------------------------------------" << endl;
  }
};

#endif