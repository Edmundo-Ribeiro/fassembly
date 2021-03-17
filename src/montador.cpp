#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

/**
Algoritmo da primeira passagem:
contador_posição = 0
contador_linha = 1
Enquanto arquivo fonte não chegou ao fim, faça:
  Obtém uma linha do fonte
  Separa os elementos da linha:
  rótulo, operação, operandos, comentários
  
  Ignora os comentários
  
  Se existe rótulo:
    Procura rótulo na TS (Tabela de Símbolos)
  
  Se achou: Erro à símbolo redefinido
  
  Senão: Insere rótulo e contador_posição na TS
    Procura operação na tabela de instruções
  Se achou:
    contador_posição = contador_posição + tamanho da instrução
  Senão:
    Procura operação na tabela de diretivas
  Se achou:
    chama subrotina que executa a diretiva
    contador_posição = valor retornado pela subrotina
  Senão: Erro, operação não identificada
  contador_linha = contador_linha + 1
*/


typedef map<string,array<int,2>> operation_table;
typedef map<string,int> symbols_table;

string errors;

bool is_label(vector<string>::const_iterator it, vector<string> &v){
  return it != v.end() - 1 && *(it+1) == ":";
}

bool is_operation(string token, operation_table &ot){
   return ot.find(token) != ot.end();
}

bool is_label_defined(string token, symbols_table &st){
   return st.find(token) != st.end();
}

vector<string>get_tokens_from_line(string line){

    vector<string> tokens; //vetor para retorno
    string aux; // armazenamento temporario dos tokens
    string separators (",:; "); // indicações de cortar string
    bool found;

    //para cada caracter na linha
    for (auto it = line.begin(); it != line.end(); ++it){
        
        found = separators.find(*it) != string::npos; //é um separador?

        if(!found) aux += toupper(*it);// se não for um separador adicionar caracter em aux
        
        if((found || it+1 == line.end()) && aux.length()){ // (se achou um separador ou é o ultimo caracter) e aux tem algum conteudo
          tokens.push_back(aux); //adicionar aux ao vetor
          if (!(*it == ' ' || it+1 == line.end())) tokens.push_back(string(1,*it)); // se o separador não for espaço e nem o ultimo caracter, adicinar o separador no vetor
          aux.clear(); // resetar aux
        }
        else if(*it == ';') tokens.push_back(string(1,*it)); //se achou um separador mas a aux não tem conteudo, colocar ";" no vetor. a linha inteira é um comentario

      }
      return tokens;
  }


int main(int argc,char **argv){
  string fname = string(argv[1]!= NULL ? argv[1] : "");
  string line;

  constexpr bool OPCODE = 0;
 
  //Tabela de operações
  // string --> Opcode, numero_de_operandos
 operation_table OT { {"ADD", {1, 1}},
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
    {"SPACE", {-1, 1}},
    {"CONST", {-1, 1}}
  };

  symbols_table ST;

  
  if(fname!=""){

    ifstream source(fname);
    ofstream destiny(fname.replace(fname.end()-3 ,fname.end(), "obj"));
    if(!source.is_open()){
      cout << "error trying to open " << fname << endl;
    }

    int line_counter = 1, position_counter = 0;
    while (!source.eof()){
      getline(source, line);
      
      // destiny << line << endl;

      // int it1 = 0;
      // int it2 = 0;
      // cout << line << "--> ";
      // for(; it1 != line.size(); ++it1){
      //   if( (line[it1] == ' ' || line[it1] =='\n')  && line[it2] != ' '){
      //     tokens.push_back(line.substr(it2,it1));
      //   }
      //   if(line[it1] == ' '){
      //     it2 = it1+1;
      //   }
        
      // }

      
      auto tks = get_tokens_from_line(line);
      for(auto it = tks.begin(); it != tks.end(); ++it){
        if(is_label(it,tks)){
          cout << "LABEL(" << *it << ")" << endl;
          if(!is_label_defined(*it,ST)){
            ST[*it] = position_counter;
          }
          else{
            stringstream message;
            message << "\nErro <Tipo>: Definição repetida de simbolo " << position_counter << "\t" << line_counter;
            errors.append(message.str());
          }

        }
        else if(is_operation(*it,OT)){
          position_counter += OT[*it][!OPCODE];
        }
        else{
          stringstream message;
          message << "\nErro <Tipo>: Operação ( " << *it << ") não existe. "<<position_counter << " " << line_counter;
          errors.append(message.str());
        }    

        ++line_counter;     
      }

      // cout << line << " ->> ";
			// it = 	sregex_token_iterator(line.begin(), line.end(), re);
			// for (; it != reg_end ; ++it) {
			// 	token = it->str();

      //   if(is_label(token)) cout << "LABEL("<<token<<")  " ;
      //   else if(is_operation(token,OT)) cout << "OPERATION("<<token<<")  " ;
      //   else cout << it->str() << "\t";
			// }
			// cout << endl;
    }
    source.close();
    destiny.close();

    for (const auto& m : ST)
      cout << "m[" << m.first << "] = (" << m.second << ") " << endl;

  }
  return 0;
}