#include <iostream>
#include <fstream>
#include <map>
#include<sstream>
#include "../../include/errors.h"
using namespace std;

 //Tabela de operações
  // string --> Opcode, numero_de_operandos
typedef map<string,array<int,2>> operation_table;

typedef map<string,int> symbols_table;

typedef vector<string>::iterator vsit;

constexpr bool OPCODE = 0;
 
  
symbols_table ST;

operation_table OT { 
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
    {"CONST", {-1, 1}}
  };

//guardar erros
ERRORS e; 

//Nome já diz, contador de linha e contador de posição
int line_counter = 1, position_counter = 0;

//Verificar se string é um numero inteiro
bool is_int(string s){
  auto start = (s[0] == '-' ? s.begin() + 1 : s.begin());

  for(auto c = start; c != s.end(); ++c){
    if(*c > '9' || *c < '0') return false; 
  }
  return true;
}

//verificar as regras de construção de uma label
bool is_label_ok(string label){
  bool flag = true;
  if(label[0] >= '0' && label[0]  <= '9') {
    flag = false;
    e.add(e.LEXICO, line_counter,"{" + label +"} Label não pode iniciar com números");   
  }
  if(label.size() > 50){
    flag = false;
    e.add(e.LEXICO, line_counter,"{" + label +"} Label não pode ser maior que 50 caracteres");   
  }

  for (auto it = label.begin(); it != label.end(); ++it ){
    if( (*it < 'A' || *it > 'Z') && (*it < '0' || *it > '9') && *it != '_'){
      flag = false;
      e.add(e.LEXICO, line_counter,"{" + label +"} Label pode conter apenas caracteres alfanúmericos e \"_\" ");
      break;
    } 
  }

  return flag;
}

//Verificar se token é uma operação
bool is_operation(string token, operation_table &ot){
   return ot.find(token) != ot.end();
}
//Verificar se label já está definida
bool is_label_defined(string token, symbols_table &st){
   return st.find(token) != st.end();
}

//Vefificar se está ocorrendo uma definicação de label 
bool is_label_definition(vsit it, vsit itend){
  return it != itend-1 && *(it+1) == ":";
}


string classifier(vector<string> tokens);
string check_for_next_n_operators(int n, vsit it, vsit itend);

//verificar se apos operação o numero certo de operandos está presente
string check_for_next_n_operators(int n, vsit it, vsit itend){
  vector<string> aux(it+1,itend);
  string operands = classifier(aux);
  if(n == 2 ){
    if(operands == "P,P" || operands == "P,PC")
      return operands;
    else if (operands == "PP")
        e.add(e.SINTATICO, line_counter,"Está faltando \",\" entre os operandos da operação {" + *it +"}");

  }
  if(n == 1 && (operands == "P" || operands == "PC")) {
    return operands;
  }
  
  e.add(e.SINTATICO, line_counter,"Número indevidos de operandos. Operação {" + *it +"} requer " +to_string(n) + " operando" + (n > 1? "s" : ""));

  return ""; //jogar erros de numero de operandos
}


//cria string classificando os tokens (tbm verifica erros e cria tabela de simbolos)
string classifier(vector<string> tokens){
  string anottend;
  int n_ops = 0;

  for(auto it = tokens.begin(); it != tokens.end(); ++it){
    if(is_label_definition(it, tokens.end())){
      if(is_label_ok(*it)){
        if(!is_label_defined(*it,ST)) 
          ST[*it] = position_counter;
        else
          e.add(e.SEMANTICO,line_counter,"Redefinição de simbolo { " + *it + " }.");
      }
      anottend += "L";
      ++it;
    }
    else if(is_operation(*it,OT)){
        if(*it == "CONST"){
          if(is_int(*(it+1))){
            n_ops = 1;
          }
          else{
            e.add(e.LEXICO,line_counter,"Diretiva CONST requer número inteiro como parametro, não {" + *(it+1) + "}");
          }
        }
        
        position_counter += OT[*it][!OPCODE];
        n_ops += OT[*it][!OPCODE] - 1;

        anottend += ("O" + to_string(n_ops));
        
        if(n_ops != 0){
            string res = check_for_next_n_operators(n_ops,it,tokens.end());
            anottend += res;
            it=tokens.end()-1;
        }
    }
    else if(*it == ";"){
      anottend+= "C";
      break;
    }
    else if(*it == ","){
      anottend+=",";
    }
    else
      anottend += "P";
    
  }
  return anottend;

}

//De uma linha do arquivo fonte, separa todos os tokens
vector<string>get_tokens_from_line(string line){

    vector<string> tokens; //vetor para retorno
    string aux; // armazenamento temporario dos tokens
    string separators (",:; "); // indicações de cortar string
    bool found;

    //para cada caracter na linha
    for (auto it = line.begin(); it != line.end(); ++it){
        
        found = (separators.find(*it) != string::npos || *it == '\t'); //é um separador?
       
        if(!found) aux += toupper(*it);// se não for um separador adicionar caracter em aux
        
        if((found || it+1 == line.end()) && aux.length()){ // (se achou um separador ou é o ultimo caracter) e aux tem algum conteudo
          if(aux == "SECTION"){
            aux.clear();
            break;
          } 
          else{
            tokens.push_back(aux); //adicionar aux ao vetor
            if ((!(*it == ' ' || (it+1) == line.end()) )|| *it ==':') tokens.push_back(string(1,*it)); // se o separador não for espaço e nem o ultimo caracter, adicinar o separador no vetor

          }
          aux.clear(); // resetar aux
        }
        else if(*it == ';') tokens.push_back(string(1,*it)); //se achou um separador mas a aux não tem conteudo, colocar ";" no vetor. a linha inteira é um comentario

      }
      return tokens;
  }

//realiz a primeira passagem e retorna "arquivo" intermediario
stringstream first_pass(ifstream &source){
  string line;
  string abbreviation;
  vsit it;
  stringstream temp;
  
  while (!source.eof()){
    getline(source, line);
    
    auto tokens = get_tokens_from_line(line);

    if(tokens.empty()){
      ++line_counter;
      temp << endl;
      continue;
    } 

    abbreviation = classifier(tokens);
    ++line_counter;

    it = tokens.begin();
    for(auto c = abbreviation.begin(); c != abbreviation.end(); ++c){
        switch (*c){
          case 'L': it+=2; break;
          
          case 'O': ++c; temp << *it << " "; ++it;break;
          
          case 'C': break;

          case ',': ++it; continue;
          
          default:
            temp << *it << " ";
            it += it+1 == tokens.end() ? 0 : 1;
            break;
        }
    }
    temp << endl;
  }
  source.close();
  line_counter = 1;
  position_counter = 0;
  return temp;
}
//realiza segunda passagem e retonar dados para o arquivo .obj
stringstream second_pass(stringstream &temp){
  string line;
  vector<string> tokens;
  stringstream output_content;

  while (!temp.eof()){
    getline(temp,line);
    tokens = get_tokens_from_line(line);

    if(tokens.empty()){
      ++line_counter;
      continue;
    } 

    for(auto it = tokens.begin(); it != tokens.end(); ++it){
      if(is_operation(*it, OT)){
        auto op_info = OT[*it];

        if(*it == "CONST"){
          output_content << *(it+1) << " ";
          position_counter += op_info[!OPCODE];
          ++it;
        }
        else{
          output_content << op_info[OPCODE] << " ";
          position_counter += op_info[!OPCODE];   
          if(op_info[!OPCODE] != 1){
            for(int i = 0; i < op_info[!OPCODE]-1; ++i){
              it++;
              if(is_label_defined(*it, ST)){
                output_content << ST[*it] << " ";
              }
              else{
                e.add(e.SEMANTICO,line_counter,"Simbolo {" + *it + "} não está definido.");
              }
            }         
          }
        }
      }
      else{
        e.add(e.SEMANTICO,line_counter,"Operação {" + *it + "} não existe.");
      }
    }
    ++line_counter;
  }
  return output_content;
}

int main(int argc,char **argv){
  string fname = string(argv[1]!= NULL ? argv[1] : "");
  stringstream temp_data,obj_data;
  
  ifstream source(fname);
  
  if(fname == ""){
    cout << "É necessário informar um arquivo .asm" << endl;
    return 0;
  }

  if(!source.is_open()){
    cout << "Erro ao tentar abrir: " << fname << endl;
    return 0;
  }   

  temp_data = first_pass(source);
  obj_data = second_pass(temp_data);

  if(e.any)
    e.show();
  else{
    ofstream destiny(fname.replace(fname.end()-3 ,fname.end(), "obj"));
    destiny << obj_data.str();
    destiny.close();
  }
 
  return 0;
}