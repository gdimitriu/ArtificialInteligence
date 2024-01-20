CXX = g++
SOURCES = Perceptron.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS =  $(SOURCES:%.cpp=%.h)
EXECUTABLES = $(OBJECTS) Perceptron_test

FLAGS = -std=gnu++23 -ggdb

all: $(EXECUTABLES)

Perceptron.o: Perceptron.cpp Perceptron.h
	$(CXX) $(FLAGS) -c Perceptron.cpp

Perceptron_test: Perceptron_test.cpp Perceptron.o
	$(CXX) $(FLAGS) -o Perceptron_test Perceptron_test.cpp Perceptron.o
	
clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done