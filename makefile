SOURCES=src/
BUILD=build/
HEADER=include/
OPTPARSER=OptParser/
BIN=bin/

CXXFLAGS+=-I$(HEADER)
CXXFLAGS+=-I$(OPTPARSER)
LFLAGS+=-I$(HEADER)
LFLAGS+=-I$(OPTPARSER)

SOURCE_FILES=$(wildcard $(SOURCES)/*.cpp)
OBJ_FILES := $(addprefix $(BUILD),$(notdir $(SOURCE_FILES:.cpp=.o)))
OBJ_FILES+=$(BUILD)OptParser.o

all: a2os
	

a2os: $(BUILD) $(BIN) $(BUILD)AutomatonToOmegaSemigroup.o
	$(CXX) -o $(BIN)a2os $(OBJ_FILES) $(BUILD)AutomatonToOmegaSemigroup.o $(LFLAGS)
	
$(BUILD)AutomatonToOmegaSemigroup.o: AutomatonToOmegaSemigroup.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCES)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(BUILD)OptParser.o: $(OPTPARSER)OptParser.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BUILD)OptParser.o $<
	
$(BUILD):
	mkdir $(BUILD)
	
$(BIN):
	mkdir $(BIN)
	
clean: $(BUILD) $(BIN)
	rm -r $(BUILD); rm -r $(BIN)