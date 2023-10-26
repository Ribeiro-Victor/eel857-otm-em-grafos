CXX = g++				# Compilador C++
CXXFLAGS = -std=c++11 	# Versão do C++ e flags
SRC_FILES = main.cpp helper.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
OUTPUT = output.out

all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OUTPUT) $(OBJ_FILES)