# Compilador C++ e flags
CXX := g++
CXXFLAGS := -std=c++11 -Wall -Iinclude
LDFLAGS :=

# Diretórios
SRCDIR := src
INCLUDEDIR := include
OBJDIR := obj
BINDIR := bin

# Arquivos fonte e objeto
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))
EXECUTABLE := $(BINDIR)/tp3.out

# Alvo principal: compila tudo
all: $(EXECUTABLE)

# Regra para criar o executável
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Executável '$(EXECUTABLE)' criado com sucesso."

# Regra para compilar arquivos objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Alvo para limpar arquivos compilados
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Arquivos compilados foram removidos."

.PHONY: all clean