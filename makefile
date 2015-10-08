SOURCES=src/
BUILD=build/
HEADER=include/
OPTPARSER=OptParser/
BIN=bin/
HAVEL=vanHavel/

INCLUDE=-I$(HEADER) -I$(addprefix $(HEADER),Automata) -I$(addprefix $(HEADER),TransitionProfiles) -I$(HAVEL) -I$(OPTPARSER)

CXXFLAGS+=$(INCLUDE)
CXXFLAGS+=-std=c++11
CXXFLAGS+=-Wall #debug
CXXFLAGS+=-g #debug
LFLAGS+=$(INCLUDE)

SOURCE_FILES=$(wildcard $(SOURCES)/*.cpp) 
SOURCE_FILES+=$(wildcard $(SOURCES)/Automata/*.cpp)

OBJ_FILES=$(addprefix $(BUILD),$(notdir $(SOURCE_FILES:.cpp=.o)))
OBJ_FILES+=$(BUILD)OptParser.o
OBJ_FILES+=$(BUILD)vanHavel_Util.o $(BUILD)UnionFind.o

all: a2os
	
a2os: $(BUILD) $(BIN) $(BUILD)AutomatonToOmegaSemigroup.o
	$(CXX) $(CXXFLAGS) -o $(BIN)a2os $(OBJ_FILES) $(BUILD)AutomatonToOmegaSemigroup.o $(LFLAGS)
	
$(BUILD)AutomatonToOmegaSemigroup.o: AutomatonToOmegaSemigroup.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)%Automaton.o: $(SOURCES)Automata/%Automaton.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCES)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(BUILD)OptParser.o: $(OPTPARSER)OptParser.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BUILD)OptParser.o $<
	
$(BUILD)vanHavel_Util.o: $(HAVEL)vanHavel_Util.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BUILD)vanHavel_Util.o $<
	
$(BUILD)UnionFind.o: $(HAVEL)UnionFind.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BUILD)UnionFind.o $<
	
$(BUILD):
	mkdir $(BUILD)
	
$(BIN):
	mkdir $(BIN)
	
clean: $(BUILD) $(BIN)
	rm -r $(BUILD); rm -r $(BIN)