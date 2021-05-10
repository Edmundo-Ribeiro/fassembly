
#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>
#include <vector>

class ERRORS {
 public:
  std::vector<std::string> messages;
  bool any = false;
  std::string error_types[3] = {"Semântico", "Sintático", "Léxico"};

  const uint8_t SEMANTICO = 0;
  const uint8_t SINTATICO = 1;
  const uint8_t LEXICO = 2;

  void add(uint8_t type, std::string message) {
    any = true;
    messages.push_back("Erro " + error_types[type] + "--> " + message);
  }

  void add(uint8_t type, int line, std::string message) {
    any = true;
    messages.push_back("Erro " + error_types[type] + " na linha " + std::to_string(line) + ", --> " + message);
  }

  void show() {
    for (auto it = messages.begin(); it != messages.end(); ++it) {
      std::cout << *it << std::endl;
    }
  }

  void clear() {
    any = false;
    messages.clear();
  }
};

#endif