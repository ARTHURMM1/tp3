# Compilador C++ a ser utilizado
CXX := g++

# Flags de compilação, incluindo o padrão C++11
CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude

# Diretórios
SRCDIR := src
INCLUDEDIR := include
OBJDIR := obj
BINDIR := bin

# Encontra todos os arquivos .cpp no diretório src
SOURCES := $(wildcard $(SRCDIR)/*.cpp)

# Gera os nomes dos arquivos objeto a partir dos fontes
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Nome do executável final
TARGET := $(BINDIR)/tp3.out

# Regra principal: compila tudo
all: $(TARGET)

# Regra para linkar os objetos e criar o executável
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar cada arquivo .cpp em um arquivo .o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Regra para limpar os arquivos gerados
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Phony targets não representam arquivos
.PHONY: all clean