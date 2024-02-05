CXX = g++

SOURCES = 
OBJECTS = $(SOURCES:%_test.cpp=%_test.o)
HEADERS = $(SOURCES:%.cpp=%.h)

FLAGS = -std=gnu++23

LIBS = -lgtest -lgtest_main -lpthread -lm -lmatrix -L ../../libmatrix/lib

EXECUTABLES = runTests

all: $(OBJECTS) $(EXECUTABLES)

runTests: runTestsMain.cpp
	$(CXX) -fPIC -o runTests runTestsMain.cpp $(OBJECTS) $(LIBS) $(FLAGS)

clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	for object in $(OBJECTS);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done