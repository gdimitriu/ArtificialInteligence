CXX = g++
SOURCES = matrixf.cpp matrix.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS =  $(SOURCES:%.cpp=%.h)
EXECUTABLES = $(OBJECTS) libmatrix.so

FLAGS = -std=gnu++23 -ggdb -fPIC

all: $(EXECUTABLES)

matrix.o: matrix.cpp ./include/matrix.h
	$(CXX) $(FLAGS) -c matrix.cpp

matrixf.o: matrixf.cpp ./include/matrixf.h matrix.o
	$(CXX) $(FLAGS) -c matrixf.cpp

libmatrix.so: $(OBJECTS)
	$(CXX) $(FLAGS) -shared -o lib/libmatrix.so $(OBJECTS)
	
clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	if [ -f lib/libmatrix.so ]; then rm lib/libmatrix.so; fi;
	
