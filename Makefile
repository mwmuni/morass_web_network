CXX ?= clang++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

TARGET := morass
TEST_TARGET := morass_tests
SOURCES := MorassWebNetwork.cpp morassnetwork.cpp geneticalgorithm.cpp node.cpp
TEST_SOURCES := tests.cpp morassnetwork.cpp node.cpp

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET) 100 100

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)
