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

bool is_label(string token){
	return token.back() == ':';
}
bool is_operation(string token, operation_table &ot){
   return ot.find(token) != ot.end();
}

vector<string>get_tokens_from_line(string line){

    vector<string> tokens; //vetor para retorno
    string aux; // armazenamento temporario dos tokens
    string separators (",:; "); // indicações de cortar string
    bool found;

    //para cada caracter na linha
    for (auto it = line.begin(); it != line.end(); ++it){
        
        found = separators.find(*it) != string::npos; //é um separador?

        if(!found) aux += *it;// se não for um separador adicionar caracter em aux
        
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
    {"SUB", {2, 1}},
    {"MUL", {3, 1}},
    {"DIV", {4, 1}},
    {"JMP", {5, 1}},
    {"JMPN", {6, 1}},
    {"JMPP", {7, 1}},
    {"JMPZ", {8, 1}},
    {"COPY", {9, 2}},
    {"LOAD", {10, 1}},
    {"STORE", {11, 1}},
    {"INPUT", {12, 1}},
    {"OUTPUT", {13, 1}},
    {"STOP", {14, 0}},
    {"SPACE", {-1, 0}},
    {"CONST", {-1, 1}}
  };

  symbols_table st;


  if(fname!=""){

    ifstream source(fname);
    ofstream destiny(fname.replace(fname.end()-3 ,fname.end(), "obj"));
    if(!source.is_open()){
      cout << "error trying to open " << fname << endl;
    }

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

      
      cout << line << " --> ";
      auto tks = get_tokens_from_line(line);
      for(auto it = tks.begin(); it != tks.end(); ++it){
        cout << *it << "|" ;
      }
      cout << endl;

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


  }
  return 0;
}