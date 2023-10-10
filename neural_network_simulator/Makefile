CXX = g++
CC = gcc

SOURCES = matrixf.cpp matrix.cpp netfeedf.cpp netfeedl.cpp netfeedft.cpp netfeedftp.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS = $(SOURCES:%.cpp=%.h)
FLAGS = -std=gnu++11 -fPIC
LIBS = 
EXECUTABLES = libneuralnetwork.so 
all: $(OBJECTS) $(EXECUTABLES)	
	cd unitests && make

libneuralnetwork.so: matrixf.cpp matrix.cpp netfeedf.cpp netfeedl.cpp netfeedft.cpp netfeedftp.cpp
	$(CXX) $(FLAGS) -shared -o lib/libneuralnetwork.so matrixf.cpp matrix.cpp netfeedf.cpp netfeedl.cpp netfeedft.cpp netfeedftp.cpp
clean:
	cd unitests && make clean
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	for object in $(OBJECTS);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	if [ -f lib/libneuralnetwork.so ]; then rm lib/libneuralnetwork.so; fi;