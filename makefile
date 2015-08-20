SOURCES=src/
BUILD=build/
HEADER=include/

CXXFLAGS=-I$(HEADER)
LFLAGS=-I$(HEADER)

SOURCE_FILES=$(wildcard $(SOURCES)/*.cpp)
OBJ_FILES := $(addprefix $(BUILD),$(notdir $(SOURCE_FILES:.cpp=.o)))

all: clean B20S
	

B2OS: $(BUILD) $(BUILD)BuechiToOmegaSemigroup.o
	$(CXX) -o $(BUILD)B2OS $(OBJ_FILES) $(BUILD)BuechiToOmegaSemigroup.o $(LFLAGS)
	
$(BUILD)BuechiToOmegaSemigroup.o: BuechiToOmegaSemigroup.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD)%.o: $(SOURCES)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD):
	mkdir $(BUILD)
	
clean:
	rm -r $(BUILD)