CXX := g++
CXXFLAGS := -std=c++11 -static-libgcc
LFLAGS := $(CXXFLAGS)
LIBS := -lglfw3 -lopengl32 -lglu32 -lgdi32 -luser32 -lkernel32
INCLUDES := -I. -Iinclude

TARGET := vis.exe

SRCDIR := src
OBJDIR := obj

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Assembling plugin..."
	$(CXX) -o $@ $(LFLAGS) $^ $(LIBS)
	@echo "\n-------> Created "$@" successfully! <-----\n"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INCLUDES)
	@echo "--------> Compiled "$<" successfully! <---------\n"

.PHONY: clean

clean:
	-rm -f $(OBJDIR)/*.o $(TARGET); clear

