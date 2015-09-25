SOURCES=src/
BUILD=build/
HEADER=include/
OPTPARSER=OptParser/
BIN=bin/
UTIL=util/

CXXFLAGS+=-I$(HEADER)
CXXFLAGS+=-I$(OPTPARSER)
CXXFLAGS+=-I$(UTIL)
CXXFLAGS+="-std=c++11"
CXXFLAGS+=-Wall
CXXFLAGS+=-g
LFLAGS+=-I$(HEADER)
LFLAGS+=-I$(OPTPARSER)
LFLAGS+=-I$(UTIL)

SOURCE_FILES=$(wildcard $(SOURCES)/*.cpp)
OBJ_FILES := $(addprefix $(BUILD),$(notdir $(SOURCE_FILES:.cpp=.o)))
OBJ_FILES+=$(BUILD)OptParser.o
OBJ_FILES+=$(BUILD)dasDull_Util.o

all: a2os

a2os: $(BUILD) $(BIN) $(BUILD)AutomatonToOmegaSemigroup.o
	$(CXX) -o $(BIN)a2os $(OBJ_FILES) $(BUILD)AutomatonToOmegaSemigroup.o $(LFLAGS)
	
$(BUILD)AutomatonToOmegaSemigroup.o: AutomatonToOmegaSemigroup.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCES)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(BUILD)OptParser.o: $(OPTPARSER)OptParser.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BUILD)OptParser.o $<
	
$(BUILD)dasDull_Util.o: $(UTIL)dasDull_Util.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BUILD)dasDull_Util.o $<
	
$(BUILD):
	mkdir $(BUILD)
	
$(BIN):
	mkdir $(BIN)
	
clean: $(BUILD) $(BIN)
	rm -r $(BUILD); rm -r $(BIN)