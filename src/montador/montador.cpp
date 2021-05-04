#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#include "../../include/errors.h"
using namespace std;

//Tabela de operações
// string --> Opcode, tamanho
typedef map<string, array<int, 2>> operation_table;

typedef map<string, int> symbols_table;
typedef multimap<string, int> extern_use_table;
typedef vector<string>::iterator vsit;

constexpr bool OPCODE = 0;

symbols_table data_table;
symbols_table text_table;
symbols_table definition_table;
extern_use_table use_table;
operation_table operations{
    {"ADD", {1, 2}},
    {"SUB", {2, 2}},
    {"MUL", {3, 2}},
    {"DIV", {4, 2}},
    {"JMP", {5, 2}},
    {"JMPN", {6, 2}},
    {"JMPP", {7, 2}},
    {"JMPZ", {8, 2}},
    {"COPY", {9, 3}},
    {"LOAD", {10, 2}},
    {"STORE", {11, 2}},
    {"INPUT", {12, 2}},
    {"OUTPUT", {13, 2}},
    {"STOP", {14, 1}},
    {"SPACE", {0, 1}},
    {"CONST", {-1, 2}},
    {"BEGIN", {0, 0}},
    {"END", {0, 0}}};

//guardar erros
ERRORS e;

//Nome já diz, contador de linha e contador de posição
int line_counter = 1, position_counter = 0;

//Verificar se string é um numero inteiro
bool is_int(string s) {
  if (s.empty()) return false;

  auto start = (s[0] == '-' ? s.begin() + 1 : s.begin());

  for (auto c = start; c != s.end(); ++c) {
    if (*c > '9' || *c < '0') return false;
  }

  return true;
}

//verificar as regras de construção de uma label
bool is_label_ok(string label) {
  bool flag = true;
  if (label[0] >= '0' && label[0] <= '9') {
    flag = false;
    e.add(e.LEXICO, line_counter, "{" + label + "} Label não pode iniciar com números");
  }
  if (label.size() > 50) {
    flag = false;
    e.add(e.LEXICO, line_counter, "{" + label + "} Label não pode ser maior que 50 caracteres");
  }

  for (auto it = label.begin(); it != label.end(); ++it) {
    if ((*it < 'A' || *it > 'Z') && (*it < '0' || *it > '9') && *it != '_') {
      flag = false;
      e.add(e.LEXICO, line_counter, "{" + label + "} Label pode conter apenas caracteres alfanúmericos e \"_\" ");
      break;
    }
  }

  return flag;
}

//Verificar se token é uma operação
bool is_operation(string token, operation_table &ot) {
  return ot.find(token) != ot.end();
}
//Verificar se label já está definida
bool is_label_defined(string token, symbols_table &st) {
  return st.find(token) != st.end();
}

//Vefificar se está ocorrendo uma definicação de label
bool is_label_definition(vsit it, vsit itend) {
  return it != itend - 1 && *(it + 1) == ":";
}

void print_table(symbols_table &st) {
  for (auto &x : st) {
    std::cout << "[" << x.first << "] : [" << x.second << "]" << endl;
  }
  std::cout << endl;
}
void print_table(extern_use_table &st) {
  for (auto &x : st) {
    std::cout << "[" << x.first << "] : [" << x.second << "]" << endl;
  }
  std::cout << endl;
}

string serialize_table(symbols_table st) {
  stringstream output;
  output << st.size() << " ";
  for (auto const &x : st) {
    output << x.first << " " << x.second << " ";
  }
  return output.str();
}

string serialize_table(extern_use_table &ext) {
  stringstream output;
  uint8_t count = 0;
  for (auto const &x : ext) {
    if (x.second != -1) {
      output << x.first << " " << x.second << " ";
      ++count;
    }
  }

  return to_string(count) + " " + output.str();
}

string classifier(vector<string> tokens);
string check_for_next_n_operators(int n, vsit it, vsit itend);

//a partir de uma posição dada procurar os operandos no restante do vetor e verificar se correspondem ao numero de operandos da operação
string check_for_next_n_operators(int n, vsit it, vsit itend) {
  vector<string> aux(it + 1, itend);

  string rest_of_vector = classifier(aux);

  if (n == 2) {
    if (rest_of_vector == "P,P" || rest_of_vector == "P,PC") {
      return "2" + rest_of_vector;
    }

    else if (rest_of_vector == "PP" || rest_of_vector == "PPC") {
      e.add(e.SINTATICO, line_counter, "Está faltando \",\" entre os operandos da operação {" + *it + "}");
      return "2" + rest_of_vector;
    }
  }

  if (n == 1 && (rest_of_vector == "P" || rest_of_vector == "PC")) {
    return "1" + rest_of_vector;
  }

  if (n == 0 && (rest_of_vector == "" || rest_of_vector == "C"))
    return "0" + rest_of_vector;

  e.add(e.SINTATICO, line_counter, "Número indevidos de operandos. Operação {" + *it + "} requer " + to_string(n) + " operando" + (n > 1 ? "s." : "."));

  return to_string(rest_of_vector.size()) + rest_of_vector;
}

//variaveis para lidar com casos em que section data vem antes de text
int data_starts = -1;
int text_starts = -1;
int shift_position_data = 0;
int shift_position_text = 0;
bool in_data_section = false;
//flag par saber se contem begin e end
int begin_end = 0;  // 0:no begin no end; 1:begin end; 2:begin no end; -1: no begin end

//De uma linha do arquivo fonte, separa todos os tokens
vector<string> get_tokens_from_line(string line) {
  vector<string> tokens;      //vetor para retorno
  string aux;                 // armazenamento temporario dos tokens
  string separators(",:; ");  // indicações de cortar string
  bool found;
  bool flag_section = false;
  //para cada caracter na linha
  for (auto it = line.begin(); it != line.end(); ++it) {
    found = (separators.find(*it) != string::npos || *it == '\t');  //é um separador?

    if (!found) aux += toupper(*it);  // se não for um separador adicionar caracter em aux

    if ((found || it + 1 == line.end()) && aux.length()) {  // (se achou um separador ou é o ultimo caracter) e aux tem algum conteudo
      if (aux == "SECTION") {
        aux.clear();
        flag_section = true;
        continue;
      } else if (flag_section && aux == "DATA") {
        aux.clear();
        in_data_section = true;
        flag_section = false;
        data_starts = line_counter;
        shift_position_text = position_counter;
        break;
      } else if (flag_section && aux == "TEXT") {
        aux.clear();
        flag_section = false;
        in_data_section = false;
        shift_position_data = position_counter;
        shift_position_text = position_counter - shift_position_text;
        text_starts = line_counter;
        break;

      } else if (aux == "BEGIN") {
        begin_end = 2;
      } else if (aux == "END") {
        begin_end -= 1;
      } else {
        tokens.push_back(aux);                                                                          //adicionar aux ao vetor
        if ((!(*it == ' ' || (it + 1) == line.end())) || *it == ':') tokens.push_back(string(1, *it));  // se o separador não for espaço e nem o ultimo caracter, adicinar o separador no vetor
      }
      aux.clear();  // resetar aux
    } else if (*it == ';')
      tokens.push_back(string(1, *it));  //se achou um separador mas a aux não tem conteudo, colocar ";" no vetor. a linha inteira é um comentario
  }
  return tokens;
}

//cria string classificando os tokens (tbm verifica erros e cria tabela de simbolos)
string classifier(vector<string> tokens) {
  string abbreviation = "";
  int n_ops = 0;
  symbols_table *destination_table = &text_table;

  for (auto token_it = tokens.begin(); token_it != tokens.end(); ++token_it) {
    if (is_label_definition(token_it, tokens.end())) {
      if (is_label_ok(*token_it)) {
        if (in_data_section) {
          destination_table = &data_table;
        }

        if (!is_label_defined(*token_it, data_table) && !is_label_defined(*token_it, text_table))
          (*destination_table)[*token_it] = position_counter;
        else
          e.add(e.SEMANTICO, line_counter, "Redefinição de simbolo { " + *token_it + " }.");
      }
      abbreviation += "L";
      ++token_it;
    } else if (*token_it == "EXTERN") {
      destination_table = &text_table;
      (*destination_table)[*(token_it - 2)] = 0;
      use_table.insert(pair<string, int>(*(token_it - 2), -1));  // lista de variaveis externas
      abbreviation += "LC";

    } else if (*token_it == "PUBLIC") {
      definition_table[*(token_it + 1)] = 0;
      abbreviation += "CL";
      ++token_it;
    } else if (is_operation(*token_it, operations)) {
      position_counter += *token_it != "CONST" ? operations[*token_it][!OPCODE] : 1;

      n_ops += operations[*token_it][!OPCODE] - 1;                       // numero de operando geralmente é o tamanho - 1
      n_ops = (*token_it == "BEGIN" || *token_it == "END") ? 0 : n_ops;  // nesse caso é importante forçar o numero de operando como sendo 0
      abbreviation += ("O");                                             // identificar na abreviação uma operacao

      string res = check_for_next_n_operators(n_ops, token_it, tokens.end());

      if (*token_it == "CONST" && res[0] != '0') {
        if (!is_int(*(token_it + 1))) {
          e.add(e.SINTATICO, line_counter, "Diretiva CONST requer número inteiro como parametro, não {" + *(token_it + 1) + "}");
          res[0] = '0';
        }
      }

      abbreviation += res;
      break;
    } else if (*token_it == ";") {
      abbreviation += "C";
      break;
    } else if (*token_it == ",") {
      abbreviation += ",";
    } else
      abbreviation += "P";
  }
  return abbreviation;
}

//realiz a primeira passagem e retorna "arquivo" intermediario
stringstream first_pass(ifstream &source) {
  string line;
  string abbreviation;
  vsit token_it;
  stringstream temp;
  int found_operands, number_of_operands;
  bool invert_content;

  while (!source.eof()) {
    getline(source, line);

    auto tokens = get_tokens_from_line(line);

    if (tokens.empty()) {
      ++line_counter;
      temp << endl;
      continue;
    }

    abbreviation = classifier(tokens);

    ++line_counter;

    token_it = tokens.begin();
    int i = 0;
    while (i < abbreviation.size()) {
      switch (abbreviation[i]) {
        case 'L':
          token_it += 2;
          break;  // se for uma defnição de label, pular e ignorar

        case 'C':
          break;  //se for um comentario, ignorar

        case ',':
          token_it += 1;
          break;  // se for uma virgula, pular e ignorar

        case 'P':  //inserir no arquivo
          temp << *token_it << " ";
          token_it += token_it + 1 == tokens.end() ? 0 : 1;
          break;

        case 'O':
          found_operands = (abbreviation[i + 1]) - '0';
          number_of_operands = operations[*token_it][!OPCODE] - 1;
          if ((*token_it != "CONST" && found_operands == number_of_operands) || (*token_it == "CONST" && found_operands == 1)) {
            temp << *token_it << " ";
            token_it += token_it + 1 == tokens.end() ? 0 : 1;
            ++i;
          } else
            i = abbreviation.size();
          break;
        default:
          break;
      }
      ++i;
    }
    /*
    for(int i = 0; i < abbreviation.size(); ++i){
        switch (abbreviation[i]){
          case 'L': token_it+=2; break;
          
          case 'O': 
            ++i;
            found_operands = (abbreviation[i]) - '0';
            if((found_operands == operations[*token_it][!OPCODE]-1 && *token_it != "CONST")|| (*token_it == "CONST" && found_operands == 1)){
              temp << *token_it << " "; 
              ++token_it;
            }
            else{
              i = abbreviation.size() - 1;
            }
            break;
          
          case 'C': break;

          case ',': ++token_it; continue;
          
          default:
            temp << *token_it << " ";
            token_it += token_it+1 == tokens.end() ? 0 : 1;
            break;
        }
    }
    */
    temp << endl;
  }

  invert_content = (data_starts < text_starts && data_starts != -1);
  if (invert_content) {
    shift_position_data = position_counter - shift_position_data;
  }

  source.close();
  line_counter = 1;
  position_counter = 0;
  return temp;
}
//realiza segunda passagem e retonar dados para o arquivo .obj
stringstream second_pass(stringstream &temp, string &bitmap) {
  string line;
  int operands;
  int table_content;
  stringstream text_bitmap;
  stringstream data_bitmap;
  stringstream *output_bitmap;
  vector<string> tokens;
  stringstream output_content;
  stringstream in_case_of_data_first;
  stringstream *output_destiny;
  bool invert_content = data_starts < text_starts && data_starts != -1;
  output_destiny = &output_content;
  output_bitmap = &text_bitmap;

  if (invert_content) {
    output_destiny = &in_case_of_data_first;
    output_bitmap = &data_bitmap;
  }

  while (!temp.eof()) {
    getline(temp, line);

    tokens = get_tokens_from_line(line);

    if (invert_content && line_counter == text_starts) {
      output_destiny = &output_content;
      output_bitmap = &text_bitmap;
    }

    if (tokens.empty()) {
      ++line_counter;
      continue;
    }

    for (auto token_it = tokens.begin(); token_it != tokens.end(); ++token_it) {
      if (is_operation(*token_it, operations)) {
        auto op_info = operations[*token_it];
        (*output_bitmap) << "0";

        if (*token_it == "CONST") {
          *(output_destiny) << *(token_it + 1) << " ";
          position_counter += (op_info[!OPCODE] - 1);
          ++token_it;
          break;
        }

        *(output_destiny) << op_info[OPCODE] << " ";

        if (op_info[!OPCODE] != 1) {
          operands = op_info[!OPCODE] - 1;
          for (int i = 0; i < operands; ++i) {
            token_it++;
            if (is_label_defined(*token_it, data_table)) {
              (*output_bitmap) << "1";
              if (invert_content)
                table_content = data_table[*token_it] + shift_position_data;
              else
                table_content = data_table[*token_it];

              if (is_label_defined(*token_it, definition_table)) {
                definition_table[*token_it] = table_content;
              }

              *(output_destiny) << table_content << " ";

            } else if (is_label_defined(*token_it, text_table)) {
              (*output_bitmap) << "0";
              cout << "is_label_defined(*token_it, text_table)" << endl
                   << "invert_content: " << invert_content << endl
                   << "text_table[" << *token_it << "]" << endl;
              if (invert_content && text_table[*token_it] != 0)
                table_content = text_table[*token_it] - shift_position_text;
              else
                table_content = text_table[*token_it];

              if (is_label_defined(*token_it, definition_table)) {
                definition_table[*token_it] = table_content;
              }

              if (use_table.find(*token_it) != use_table.end()) {
                use_table.insert(pair<string, int>(*token_it, position_counter + 1 + i - (invert_content ? shift_position_text : 0)));
              }

              *(output_destiny) << table_content << " ";
            } else {
              e.add(e.SEMANTICO, line_counter, "Simbolo {" + *token_it + "} não está definido.");
            }
          }
        }
        position_counter += op_info[!OPCODE];
      } else {
        e.add(e.SEMANTICO, line_counter, "Operação {" + *token_it + "} não existe.");
      }
    }
    ++line_counter;
  }

  for (auto &x : definition_table) {
    if (is_label_defined(x.first, text_table)) {
      x.second = text_table[x.first] - (invert_content ? shift_position_text : 0);
    }
  }

  if (invert_content) {
    output_content << in_case_of_data_first.str();
    text_bitmap << data_bitmap.str();
  }
  bitmap = text_bitmap.str();
  return output_content;
}

int main(int argc, char **argv) {
  int number_of_files = argc - 1;
  string fname;
  stringstream temp_data, obj_data;
  ifstream source;
  string bitmap;
  int obj_file_size;

  for (uint8_t i = 0; i < number_of_files; ++i) {
    fname = string(argv[i + 1] != NULL ? argv[i + 1] : "");
    source = ifstream(fname);

    if (fname == "") {
      std::cout << "É necessário informar um arquivo .asm" << endl;
      return 0;
    }

    if (!source.is_open()) {
      std::cout << "Erro ao tentar abrir: " << fname << endl;
      return 0;
    }

    temp_data = first_pass(source);

    obj_data = second_pass(temp_data, bitmap);

    if (number_of_files > 1 && begin_end != 1)
      e.add(e.SINTATICO, "Módulo {" + fname + "} não utiliza corretamente BEGIN e END");

    if (number_of_files == 1 && begin_end != 0)
      e.add(e.SINTATICO, "Arquivo {" + fname + "} utiliza BEGIN/END mas não é um modulo");

    if (e.any)
      e.show();

    else {
      obj_file_size = position_counter;
      auto file_name_position = fname.find_last_of("/");
      fname = fname.substr(file_name_position + 1);
      fname = fname.replace(fname.end() - 4, fname.end(), "");
      ofstream destiny(fname + ".obj");
      std::cout << fname << endl;
      destiny << fname << endl;
      destiny << obj_file_size << endl;
      destiny << bitmap << endl;
      destiny << serialize_table(definition_table) << endl;
      destiny << serialize_table(use_table) << endl;
      std::cout << "use_table: " << serialize_table(use_table) << endl;
      destiny << obj_data.str();
      destiny.close();
    }
    print_table(definition_table);
    print_table(data_table);
    print_table(text_table);
    print_table(use_table);
    std::cout << bitmap << endl;
    std::cout << bitmap.size() << endl;
    //resetar tudo para o proximo arquivo
    e.clear();
    data_table.clear();
    text_table.clear();
    definition_table.clear();
    use_table.clear();
    bitmap.clear();
    data_starts = -1;
    text_starts = -1;
    shift_position_data = 0;
    shift_position_text = 0;
    in_data_section = false;
    position_counter = 0;
    line_counter = 1;
    obj_file_size = 0;
  }

  return 0;
}