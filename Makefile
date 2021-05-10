all: m s l

BD= build#diretorio da saida
CXX = g++ #compilador
MAIN1 = montador#nome do arquivo principal

SRCS=$(wildcard src/$(MAIN1)/*.cpp) #pegar todos arquivos .cpp em source
OBJS=$(BD)/$(subst .cpp,.o,$(SRCS)) #definir nomes dos .o que serão criados


m: $(MAIN1)
	@echo ---------------
	@echo montador feito!
	@echo ---------------

$(MAIN1): $(OBJS)
	$(CXX) -Wall -ansi -std=c++11 $(OBJS) -o $@

$(OBJS): $(SRCS)
	$(MKDIR_P) $(dir $@)
	$(CXX) -c -Wall -ansi -std=c++11 $< -o $@


MAIN2 = simulador#nome do arquivo principal

SRCS2=$(wildcard src/$(MAIN2)/*.cpp) #pegar todos arquivos .cpp em source
OBJS2=$(BD)/$(subst .cpp,.o,$(SRCS2)) #definir nomes dos .o que serão criados

s: $(MAIN2)
	@echo ----------------
	@echo simulador feito!
	@echo ----------------

$(MAIN2): $(OBJS2)
	$(CXX) -Wall -ansi -std=c++11 $(OBJS2) -o $@

$(OBJS2): $(SRCS2)
	$(MKDIR_P) $(dir $@)
	$(CXX) -c -Wall -ansi -std=c++11 $< -o $@


MAIN3 = ligador#nome do arquivo principal

SRCS3=$(wildcard src/$(MAIN3)/*.cpp) #pegar todos arquivos .cpp em source
OBJS3=$(BD)/$(subst .cpp,.o,$(SRCS3)) #definir nomes dos .o que serão criados
HEADERS = include/obj.h
l: $(MAIN3)
	@echo ----------------
	@echo ligador feito!
	@echo ----------------

$(MAIN3): $(OBJS3)
	$(CXX) -Wall -ansi -std=c++11 $(OBJS3) -o $@

$(OBJS3): $(SRCS3) $(HEADERS)
	$(MKDIR_P) $(dir $@)
	$(CXX) -c -Wall -ansi -std=c++11 $< -o $@

MKDIR_P = mkdir -p

clean: 
	rm -rf $(BD)/*
	
clear: clean
	rm -f $(MAIN1)
	rm -f $(MAIN2)
	rm -f $(MAIN3)

discart:
	rm -f *.obj
	rm -f *.OUT
	
