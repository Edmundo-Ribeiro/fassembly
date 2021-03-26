all: m s

MAIN1 = montador#nome do arquivo principal
CXX = g++ #compilador
BD= build#diretorio da saida

SRCS=$(wildcard src/$(MAIN1)/*.cpp) #pegar todos arquivos .cpp em source
OBJS=$(BD)/$(subst .cpp,.o,$(SRCS)) #definir nomes dos .o que serão criados


m: $(MAIN1)
	@echo ---------------
	@echo montador feito!
	@echo ---------------

MAIN2 = simulador#nome do arquivo principal

SRCS2=$(wildcard src/$(MAIN2)/*.cpp) #pegar todos arquivos .cpp em source
OBJS2=$(BD)/$(subst .cpp,.o,$(SRCS2)) #definir nomes dos .o que serão criados

s: $(MAIN2)
	@echo ----------------
	@echo simulador feito!
	@echo ----------------

$(MAIN1): $(OBJS)
	$(CXX) $(OBJS) -o $@

$(MAIN2): $(OBJS2)
	$(CXX) $(OBJS2) -o $@

$(OBJS): $(SRCS)
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $< -o $@

$(OBJS2): $(SRCS2)
	$(MKDIR_P) $(dir $@)
	$(CXX) -c $< -o $@

MKDIR_P = mkdir -p

clean: 
	rm -rf $(BD)/*
clear: clean
	rm -f $(MAIN1)
	rm -f $(MAIN2)
