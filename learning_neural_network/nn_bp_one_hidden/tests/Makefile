CXX = g++
SOURCES = 
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS =  $(SOURCES:%.cpp=%.h)
EXECUTABLES = $(OBJECTS) testRandom nnbp_test

LIBS = -lmatrix -L ../../libmatrix/lib

FLAGS = -std=gnu++23 -ggdb -fPIC -I ../../libmatrix/include

all: $(EXECUTABLES)

testRandom: testRandom.cpp ../NNBP1Hidden.o
	$(CXX) $(FLAGS) -o testRandom testRandom.cpp ../NNBP1Hidden.o $(LIBS) 

nnbp_test: nnbp_test.cpp ../NNBP1Hidden.o
	$(CXX) $(FLAGS) -o nnbp_test nnbp_test.cpp ../NNBP1Hidden.o $(LIBS)
clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	
