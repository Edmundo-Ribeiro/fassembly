#include <iostream>
#include <fstream>
using namespace std;

int main(int argc,char **argv){
  string fname = argv[1];
  string line;

  ifstream source(fname);
  ofstream destiny(fname.replace(fname.end()-3 ,fname.end(), "obj"));
  if(!source.is_open()){
    cout << "error trying to open " << fname << endl;
  }

  while (!source.eof()){
    getline(source, line);
    destiny << line << endl;
    cout << line << endl;
  }
  
  source.close();
  destiny.close();

  return 0;
}