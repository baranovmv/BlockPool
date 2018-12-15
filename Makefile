CXX =clang++
CXXFLAGS += -std=gnu++11 -g

LDFLAGS += -lCppUTestExt -lCppUTest

CODE=main.cpp tests.cpp
OBJECTS=$(patsubst %.cpp, %.o, $(patsubst %.cpp, %.o, $(CODE)))

OUT=tests

all: ${OUT}

clean:
	rm -f *.o ${OUT}

${OUT}: ${OBJECTS}
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

${OBJECTS}: %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
