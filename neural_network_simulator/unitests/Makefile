CXX = g++
CC = gcc

SOURCES = matrixf_test.cpp matrix_test.cpp netfeedf_test.cpp numbersAi_test.cpp
OBJECTS = $(SOURCES:%_test.cpp=%_test.o)
HEADERS = # $(SOURCES:%.cpp=%.h)
GENERAL_DEPENDENCIES = ../matrixf.o ../matrix.o
NETWORKS =  ../netfeedf.o ../netfeedl.o ../netfeedft.o ../netfeedftp.o
FLAGS = -std=gnu++11 -Wno-write-strings
LIBS = -lgtest -lgtest_main -lpthread -lm
EXECUTABLES = runTests
all: $(OBJECTS) $(EXECUTABLES)

runTests: runTestsMain.cpp $(OBJECTS)
	g++ -fPIC -o runTests runTestsMain.cpp $(OBJECTS) $(LIBS) $(FLAGS) $(GENERAL_DEPENDENCIES) $(NETWORKS)
 
matrixf_test.o: matrixf_test.cpp $(DEPENDENCIES)
	g++ -fPIC -c -o matrixf_test.o matrixf_test.cpp $(FLAGS)

matrix_test.o: matrix_test.cpp $(DEPENDENCIES)
	g++ -fPIC -c -o matrix_test.o matrix_test.cpp $(FLAGS)

netfeedf_test.o: netfeedf_test.cpp $(GENERAL_DEPENDENCIES) $(NETWORKS)
	g++ -fPIC -c -o netfeedf_test.o netfeedf_test.cpp $(FLAGS)

numbersAi_test.o: numbersAi_test.cpp $(GENERAL_DEPENDENCIES) $(NETWORKS)
	g++ -fPIC -c -o numbersAi_test.o numbersAi_test.cpp $(FLAGS)
clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	for object in $(OBJECTS);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done