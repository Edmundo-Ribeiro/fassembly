# Fassembly
Fassembly (Fake Assembly) é como decidi chamar a linguagem de montagem "fake" criada para fins didáticos durante a disciplina de 
Software Básico (Software de Sistemas) da Universidade de Brasília.

A linguagem consiste de 14 instruções e 2 diretivas {CONST, SPACE}:

| Instrução | OPCODE |
| :---: | :---: | 
| ADD | 01 | 
| SUB | 02 | 
| MUL | 03 | 
| DIV | 04 | 
| JMP | 05 | 
| JMPN | 06 | 
| JMPP | 07 | 
| JMPZ | 08 | 
| COPY | 09 | 
| LOAD | 10 | 
| STORE | 11 | 
| INPUT | 12 | 
| OUTPUT | 13 | 
| OUTPUT | 14 | 

## Montador e Simulador
O código desse repositorio consite de um montador e um simulador para fassembly

## Baixar
Para baixar, execute na linha de comando 
```sh
git clone https://github.com/Edmundo-Ribeiro/fassembly.git
```

## Utilizar
*Para compilar o projeto é necessário ter [Make](https://www.gnu.org/software/make/) instalado em sua máquina.

Para compilar o **`montador`** e o **`simulador`** basta fazer:

```sh
#entrar no diretório baixado
cd fassembly

# compilar o montaor e simulador 
make

#compilar apenas montador
make m

#compilar apenas simulador
make s

#apagar tudo
make clear
```
Para executar o montador basta fazer:
```sh
# montar arquivo .asm
./montador <CAMINHO_ARQUIVO.asm>

# exemplo
./montador examples/fibonacci.asm
```
Com o arquivo .obj resultante do passo anterior, para executar o simulador basta fazer:

```sh
# simular arquivo .obj
./simulador <CAMINHO_ARQUIVO.obj>

# exemplo
./simulador examples/fibonacci.obj
