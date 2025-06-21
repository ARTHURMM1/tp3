
CXX := g++


CXXFLAGS := -std=c++11 -Wall -Wextra -Iinclude


SRCDIR := src
INCLUDEDIR := include
OBJDIR := obj
BINDIR := bin


SOURCES := $(wildcard $(SRCDIR)/*.cpp)


OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))


TARGET := $(BINDIR)/tp2.out


all: $(TARGET)


$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean:
	rm -rf $(OBJDIR) $(BINDIR)


.PHONY: all clean