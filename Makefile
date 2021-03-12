all: compile

MAIN = montador #nome do arquivo principal
CXX = g++ #compilador
BD= build#diretorio da saida
MD = src/$(MAIN).cpp #lugar do arquivo principal

SRCS=$(wildcard src/*.cpp) #pegar todos arquivos .cpp em source
OBJS=$(BD)/$(subst .cpp,.o,$(SRCS)) #definir nomes dos .o que serão criados

compile: $(MAIN)
	echo feito!

$(MAIN): $(OBJS)
	$(CXX) $(OBJS) -o $@

$(OBJS): $(SRCS)
	echo $@
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $< -o $@

MKDIR_P = mkdir -p

clean: 
	rm -rf $(BD)/*
clear: clean
	rm -f $(MAIN).o
