CXXFLAGS := -Wall -pthread

BIN := Project01

src_files := $(wildcard *.cpp)
obj_files := $(patsubst %.cpp,%.o,$(src_files))

.PHONY: all clean remake debug redebug

all: CXXFLAGS += -O2
all: $(BIN)

clean:
	rm -f $(BIN) *.o

debug: CXXFLAGS += -DDEBUG -g3 -O0 -fno-inline
debug: $(BIN)

remake: clean all

redebug: clean debug

$(BIN): $(obj_files)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp *.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter %.cpp,$^)
