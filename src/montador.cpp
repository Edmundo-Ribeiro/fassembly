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

 //Tabela de operações
  // string --> Opcode, numero_de_operandos


typedef map<string,array<int,2>> operation_table;
typedef map<string,int> symbols_table;

  constexpr bool OPCODE = 0;
 
 
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

typedef vector<string>::iterator vsit;

bool is_label_definition(vsit it, vsit itend){
  return it != itend - 1 && *(it+1) == ":";
}

string classifier(vector<string> tokens);

string check_for_next_n_operators(int n, vsit it, vsit itend){
  vector<string> aux(it+1,itend);
  string operands = classifier(aux);
  if(n == 2 && (operands == "P,P" || operands == "P,PC") ){
    return operands;
  }
  if(n == 1 && (operands == "P" || operands == "PC")) {
    return operands;
  }
  return ""; //jogar erros de numero de operandos
}

bool is_int(string s){
  auto start = (s[0] == '-' ? s.begin() + 1 : s.begin());

  for(auto c = start; c != s.end(); ++c){
    if(*c > '9' || *c < '0') return false;//lançar erro

  }
  return true;
}

string classifier(vector<string> tokens){
  string anottend;
  int n_ops = 0;

  cout << "Token: " ;
  for(auto it = tokens.begin(); it != tokens.end(); ++it){
    cout << *it << " ";
    if(is_label_definition(it, tokens.end())){
      //checkRules(label,*it)
      anottend += "L";
      ++it;
    }
    else if(is_operation(*it,OT)){
      if(*it == "CONST"){
        is_int(*(it+1));
        n_ops = 1;
      }
      n_ops += OT[*it][!OPCODE] - 1;
      anottend+= ("O" + to_string(n_ops));
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
    else{
      anottend += "P";
    }
  }
  if(anottend.size()){

    cout << "Anotted: ";
    for(auto i = anottend.begin(); i != anottend.end();++i){
      cout << *i;
    }
    cout << endl;
  }
  return anottend;

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
          if(aux == "SECTION"){
            aux.clear();
            break;
          } 
          else{
            tokens.push_back(aux); //adicionar aux ao vetor
            if (!(*it == ' ' || it+1 == line.end())) tokens.push_back(string(1,*it)); // se o separador não for espaço e nem o ultimo caracter, adicinar o separador no vetor

          }
          aux.clear(); // resetar aux
        }
        else if(*it == ';') tokens.push_back(string(1,*it)); //se achou um separador mas a aux não tem conteudo, colocar ";" no vetor. a linha inteira é um comentario

      }
      return tokens;
  }


int main(int argc,char **argv){
  string fname = string(argv[1]!= NULL ? argv[1] : "");
  string line;

  
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
      
      auto tks = get_tokens_from_line(line);




      if(!tks.empty()){
        classifier(tks);

      }
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

    // for (const auto& m : ST)
    //   cout << "m[" << m.first << "] = (" << m.second << ") " << endl;

  }
  return 0;
}