# Fassembly
Fassembly (Fake Assembly) é como decidi chamar a linguagem de montagem "fake" criada para fins didáticos durante a disciplina de 
Software Básico (Software de Sistemas) da Universidade de Brasília.

A linguagem consiste de 14 instruções e 4 diretivas {CONST, SPACE, EXTERN, PUBLIC}:

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

## Montador, ligador e Simulador
O código desse repositorio consite de um montador e um simulador para fassembly

## Baixar
Para baixar, execute na linha de comando 
```sh
git clone https://github.com/Edmundo-Ribeiro/fassembly.git
```

# Utilizar
*Para compilar o projeto é necessário ter [Make](https://www.gnu.org/software/make/) instalado em sua máquina.

O código foi dessenvolvido no sistema operacional **ubuntu**

Para compilar o **`montador`**, **`ligador`** e **`simulador`** basta fazer:

```sh
#entrar no diretório baixado
cd fassembly

# compilar o montador, ligador e simulador 
make

#compilar apenas montador
make m

#compilar apenas ligador
make l

#compilar apenas simulador
make s

#apagar tudo
make clear

#descartar .out e .obj
make discart
```
Para executar o montador basta fazer:
```sh
# montar arquivo .asm
./montador <CAMINHO_ARQUIVO.asm>

# exemplo
./montador examples/fibonacci.asm
```
Para montar mais de um arquivo é necessário que as diretivas **PUBLIC** e **EXTERN** estejam na parte supeior fora da SECTIONS como no seguinte exemplo:
```sh
MOD_A: BEGIN

  MOD_B: EXTERN
  PUBLIC FAT
  PUBLIC N

  SECTION DATA
	  N: SPACE
	  ONE: CONST 1

  SECTION TEXT
	  INPUT N
	  LOAD N
	  FAT: SUB ONE
	  JMPZ FIM
	  JMP MOD_B
	  FIM: OUTPUT N
	  STOP
END
```

```sh
# montar arquivo .asm
./montador <CAMINHO_ARQUIVO_1.asm> <CAMINHO_ARQUIVO_2.asm>

# exemplo
./montador examples/fat/fat_mod_A.asm examples/fat/fat_mod_B.asm
```

Se mais de um arquivo for passado para o montador a saida irá ser arquivos .obj com cabeçalhos que devem passar pelo ligador

Para executar o ligador basta fazer:
```sh
# montar arquivo .asm
./ligador <CAMINHO_DO_ARQUIVO_1.obj> <CAMINHO_DO_ARQUIVO_2.obj>

# exemplo
./ligador fat_mod_A.obj fat_mod_B.obj 
```

Com o arquivo .obj resultante do passo anterior, para executar o simulador basta fazer:

```sh
# simular arquivo .obj
./simulador <CAMINHO_ARQUIVO.obj>

# exemplo
./simulador fat_mod_A_ligado.obj
```