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

all: B2OS
	

B2OS: $(BUILD) $(BIN) OptParser $(BUILD)BuechiToOmegaSemigroup.o
	$(CXX) -o $(BIN)B2OS $(OBJ_FILES) $(BUILD)BuechiToOmegaSemigroup.o $(LFLAGS)
	
$(BUILD)BuechiToOmegaSemigroup.o: BuechiToOmegaSemigroup.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCES)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
OptParser: $(OPTPARSER)OptParser.cpp
	$(CXX) $(CXXFLAGS) -c -o $(BUILD)OptParser.o $<
	
$(BUILD):
	mkdir $(BUILD)
	
$(BIN):
	mkdir $(BIN)
	
clean: $(BUILD)
	rm -r $(BUILD)