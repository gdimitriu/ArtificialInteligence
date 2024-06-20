CXX = g++
SOURCES = 
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS =  $(SOURCES:%.cpp=%.h)
EXECUTABLES = $(OBJECTS) testRandom mlp_test

LIBS = -lmatrix -L ../../libmatrix/lib

FLAGS = -std=gnu++23 -ggdb -fPIC -I ../../libmatrix/include

all: $(EXECUTABLES)

testRandom: testRandom.cpp ../MLP1Hidden.o
	$(CXX) $(FLAGS) -o testRandom testRandom.cpp ../MLP1Hidden.o $(LIBS) 

mlp_test: mlp_test.cpp ../MLP1Hidden.o
	$(CXX) $(FLAGS) -o mlp_test mlp_test.cpp ../MLP1Hidden.o $(LIBS)
clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	
