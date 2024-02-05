CXX = g++
SOURCES = MultiLayerPerceptron.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS =  $(SOURCES:%.cpp=%.h)
EXECUTABLES = $(OBJECTS) 

FLAGS = -std=gnu++23 -ggdb -fPIC -I ../libmatrix/include

all: $(EXECUTABLES)
	cd tests && make
	cd unitests && make

MultiLayerPerceptron.o: MultiLayerPerceptron.cpp MultiLayerPerceptron.h
	$(CXX) $(FLAGS) -c MultiLayerPerceptron.cpp
	
clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	cd tests && make clean
	cd unitests && make clean
