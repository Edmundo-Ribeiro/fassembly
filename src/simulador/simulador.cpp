#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

/**
 * ler arquivo
 * ler operação
 * verificar qual operação é e quantos operandos
 * ler operandos
 * executar função
 * incrementar position counter 
 * colocar valores numa tabela posicao -> valor
 * */
using namespace std;

typedef struct {
  int *pc = NULL;
  int *acc = NULL;
  int *value1;
  int *value2;

  void clear() {
    value1 = NULL;
    value2 = NULL;
  }
} data;

void _add(data *d) {
  int value = *(d->value1);
  *(d->acc) += value;
  *(d->pc) += 2;
}
void _sub(data *d) {
  int value = *(d->value1);
  *(d->acc) -= value;
  *(d->pc) += 2;
}
void _mul(data *d) {
  int value = *(d->value1);
  *(d->acc) *= value;
  *(d->pc) += 2;
}

void _div(data *d) {
  int value = *(d->value1);
  *(d->acc) /= value;
  *(d->pc) += 2;
}

void _jmp(data *d) {
  int value = *(d->value1);
  *(d->pc) = value;
}

void _jmpz(data *d) {
  int value = *(d->acc);
  int position = *(d->value1);
  *(d->pc) = value ? *(d->pc) + 2 : position;
}
void _jmpp(data *d) {
  int value = *(d->acc);
  int position = *(d->value1);
  *(d->pc) = value > 0 ? position : *(d->pc) + 2;
}

void _jmpn(data *d) {
  int value = *(d->acc);
  int position = *(d->value1);
  *(d->pc) = value < 0 ? position : *(d->pc) + 2;
}

void _copy(data *d) {
  *(d->value2) = *(d->value1);
  *(d->pc) += 3;
}

void _load(data *d) {
  int value = *(d->value1);
  *(d->acc) = value;
  *(d->pc) += 2;
}
void _store(data *d) {
  int value = *(d->acc);
  *(d->value1) = value;
  *(d->pc) += 2;
}
void _input(data *d) {
  int value;
  cin >> value;
  *(d->value1) = value;
  *(d->pc) += 2;
}
void _output(data *d) {
  cout << "> " << *(d->value1) << endl
       << endl;
  *(d->pc) += 2;
}

typedef map<int, void (*const)(data *)> function_map;
function_map operation{
    {1, _add},
    {2, _sub},
    {3, _mul},
    {4, _div},
    {5, _jmp},
    {6, _jmpn},
    {7, _jmpp},
    {8, _jmpz},
    {9, _copy},
    {10, _load},
    {11, _store},
    {12, _input},
    {13, _output}};
map<int, int> variables;
map<int, int> op_size = {
    {1, 2},
    {2, 2},
    {3, 2},
    {4, 2},
    {5, 2},
    {6, 2},
    {7, 2},
    {8, 2},
    {9, 3},
    {10, 2},
    {11, 2},
    {12, 2},
    {13, 2},
    {14, 1}};
constexpr int OUTPUT = 13;
constexpr int STOP = 14;

int *variable_at(int position, vector<int> &codes) {
  if (variables.find(position) == variables.end())
    variables[position] = codes.at(position);

  return &variables[position];
}

int main(int argc, char **argv) {
  string fname = string(argv[1] != NULL ? argv[1] : "");

  ifstream source(fname);

  string s_code;
  vector<int> codes;
  int position_counter = 0;
  int accumulator = 0;
  int position;

  data d;
  (d.pc) = &position_counter;
  (d.acc) = &accumulator;

  if (fname == "") {
    cout << "É necessário informar um arquivo .obj" << endl;
    return 0;
  }

  if (!source.is_open()) {
    cout << "Erro ao tentar abrir: " << fname << endl;
    return 0;
  }

  ofstream destiny(fname.replace(fname.end() - 3, fname.end(), "OUT"));

  do {
    getline(source, s_code, ' ');
    codes.push_back(atoi(s_code.c_str()));
  } while (!s_code.empty());

  auto start = codes.begin();
  for (auto vector_position = codes.begin();;) {
    if (*vector_position == STOP) {
      position_counter += 1;
      cout << "PC <- " << position_counter << endl;
      cout << "ACC <- " << accumulator << endl
           << endl;
      break;
    }  // se é a instrução stop pare o simulador

    int n_ops = op_size[*vector_position];  //numepro de operando é apenas o tamanho da operação - 1

    if (n_ops != 1) {  // se tem operandos pegue o primeiro
      position = *(vector_position + 1);
      if (*vector_position > 4 && *vector_position < 9)  // se for uma OP do tipo jump
        d.value1 = &position;
      else
        d.value1 = variable_at(position, codes);
    }
    if (n_ops == 3) {  // se tem dois operando pegue o segundo
      position = *(vector_position + 2);
      d.value2 = variable_at(position, codes);
    }

    operation[*vector_position](&d);  //execute a função

    if (*vector_position == OUTPUT)  // colocar saida no arquivo
      destiny << *(d.value1) << endl;

    vector_position = start + position_counter;  // mova o iterador do vetor para a posição indicado por position counter

    cout << "PC <- " << position_counter << endl;
    cout << "ACC <- " << accumulator << endl
         << endl;

    d.clear();
  }

  source.close();
  destiny.close();

  return 0;
}