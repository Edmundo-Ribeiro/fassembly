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
symbols_table LT;

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
  if(s.empty()) return false;

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


string check_for_next_n_operators(int n, vsit it, vsit itend){

  // if(it + 1 == itend){
  //   return "0";
  // }

  vector<string> aux(it+1,itend);

  string rest_of_vector = classifier(aux);

  if(n == 2 ){
    if(rest_of_vector == "P,P" || rest_of_vector == "P,PC"){
      return "2"+rest_of_vector;
    }

    else if (rest_of_vector == "PP" || rest_of_vector == "PPC"){
      e.add(e.SINTATICO, line_counter,"Está faltando \",\" entre os operandos da operação {" + *it +"}");
      return "2" + rest_of_vector;
    }    
  }

  if(n == 1 && (rest_of_vector == "P" || rest_of_vector == "PC")) {
    return "1" + rest_of_vector;
  }

  if(n == 0 && (rest_of_vector == "" || rest_of_vector == "C"))
    return "0" + rest_of_vector;

  e.add(e.SINTATICO, line_counter,"Número indevidos de operandos. Operação {" + *it +"} requer " +to_string(n) + " operando" + (n > 1? "s" : ""));

  return to_string(rest_of_vector.size()) + rest_of_vector;



}



//variaveis para lidar com casos em que section data vem antes de text
int data_starts = -1;
int text_starts = -1;
int shift_position_data = 0;
int shift_position_text = 0;
bool in_data_section = false;

//De uma linha do arquivo fonte, separa todos os tokens
vector<string>get_tokens_from_line(string line){

    vector<string> tokens; //vetor para retorno
    string aux; // armazenamento temporario dos tokens
    string separators (",:; "); // indicações de cortar string
    bool found;
    bool flag_section = false;
    //para cada caracter na linha
    for (auto it = line.begin(); it != line.end(); ++it){
        found = (separators.find(*it) != string::npos || *it == '\t'); //é um separador?
       
        if(!found) aux += toupper(*it);// se não for um separador adicionar caracter em aux
        
        if((found || it+1 == line.end()) && aux.length()){ // (se achou um separador ou é o ultimo caracter) e aux tem algum conteudo
          if(aux == "SECTION"){
            aux.clear();
            flag_section = true;
            continue;
          } 
          else if(flag_section && aux == "DATA"){
            aux.clear();
            in_data_section = true;
            flag_section = false;
            data_starts = line_counter;
            shift_position_text = position_counter;
            break;            
          }
          else if(flag_section && aux == "TEXT"){
            aux.clear();
            flag_section = false;
            in_data_section = false;
            shift_position_data = position_counter;
            shift_position_text = position_counter - shift_position_text;
            text_starts = line_counter;
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



//cria string classificando os tokens (tbm verifica erros e cria tabela de simbolos)
string classifier(vector<string> tokens){
  string anottend = "";
  int n_ops = 0;
  symbols_table * destination_table = &LT;

  for(auto it = tokens.begin(); it != tokens.end(); ++it){
    if(is_label_definition(it, tokens.end())){
      if(is_label_ok(*it)){
        
        if(in_data_section){
          destination_table = &ST;
        }


        if(!is_label_defined(*it,ST) && !is_label_defined(*it, LT))
          (*destination_table)[*it] = position_counter;
        else
          e.add(e.SEMANTICO,line_counter,"Redefinição de simbolo { " + *it + " }.");
      }
      anottend += "L";
      ++it;
    }
    else if(is_operation(*it,OT)){
        position_counter += OT[*it][!OPCODE];
        
        n_ops += OT[*it][!OPCODE] - 1; // numero de operando geralmente é o tamanho - 1
        if( *it == "CONST") n_ops = 1; // no caso da diretiva const ela tem que ter um operando
        anottend += ("O"); // identificar na abreviação uma operacao


        string res = check_for_next_n_operators(n_ops,it,tokens.end());

        if(*it == "CONST" && res[0] != '0'){
          if(!is_int(*(it+1))){
            e.add(e.SINTATICO,line_counter,"Diretiva CONST requer número inteiro como parametro, não {" + *(it+1) + "}");
            res[0] = '0';
          }
        }

        anottend += res;
        break;

      

        
        
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


//realiz a primeira passagem e retorna "arquivo" intermediario
stringstream first_pass(ifstream &source){
  string line;
  string abbreviation;
  vsit it;
  stringstream temp;
  int n;
  bool invert_content;


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
    for(int i = 0; i < abbreviation.size(); ++i){
        switch (abbreviation[i]){
          case 'L': it+=2; break;
          
          case 'O': 
            ++i;
            n = (abbreviation[i]) - '0';
            if((n == OT[*it][!OPCODE]-1 && *it != "CONST")|| (*it == "CONST" && n == 1)){
              temp << *it << " "; 
              ++it;
            }
            else{
              i = abbreviation.size() - 1;
            }
            break;
          
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

  invert_content = data_starts < text_starts && data_starts != -1;
  if(invert_content){
    shift_position_data = position_counter - shift_position_data;
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
  stringstream in_case_of_data_first;
  stringstream * output_destiny;
  bool invert_content = data_starts < text_starts && data_starts != -1;
  output_destiny = &output_content;



  if(invert_content){

    output_destiny = &in_case_of_data_first;
  }

  while (!temp.eof()){
    getline(temp,line);


    

    tokens = get_tokens_from_line(line);

    

    if(invert_content && line_counter == text_starts){
      output_destiny = &output_content;
    }

    if(tokens.empty()){
      ++line_counter;
      continue;
    } 

    for(auto it = tokens.begin(); it != tokens.end(); ++it){
      if(is_operation(*it, OT)){
        auto op_info = OT[*it];

        if(*it == "CONST"){
          *(output_destiny) << *(it+1) << " ";
          position_counter += op_info[!OPCODE];
          ++it;
        }
        else{
          *(output_destiny) << op_info[OPCODE] << " ";
          position_counter += op_info[!OPCODE];   
          if(op_info[!OPCODE] != 1){
            for(int i = 0; i < op_info[!OPCODE]-1; ++i){
              it++;
              if(is_label_defined(*it, ST)){
                if(invert_content)
                  *(output_destiny) << ST[*it]  + shift_position_data << " ";
                else
                  *(output_destiny) << ST[*it]  << " ";
              }
              else if(is_label_defined(*it, LT)){
                if(invert_content)
                  *(output_destiny) << LT[*it] - shift_position_text  << " ";
                
                else
                  *(output_destiny) << LT[*it]  << " ";
              }
              else{
                e.add(e.SEMANTICO,line_counter,"Simbolo {" + *it + "} não está definido.");
              }
            }         
          }
        }
      }
      else{
        e.add(e.SINTATICO,line_counter,"Operação {" + *it + "} não existe.");
      }
    }
    ++line_counter;

  }


  if(invert_content){
    output_content << in_case_of_data_first.str();
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