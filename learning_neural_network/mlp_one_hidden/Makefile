CXX = g++
SOURCES = MLP1Hidden.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS =  $(SOURCES:%.cpp=%.h)
EXECUTABLES = $(OBJECTS) 

FLAGS = -std=gnu++23 -ggdb -fPIC -I ../libmatrix/include

all: $(EXECUTABLES)
	cd tests && make
	cd unitests && make

MLP1Hidden.o: MLP1Hidden.cpp MLP1Hidden.h
	$(CXX) $(FLAGS) -c MLP1Hidden.cpp
clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done
	cd tests && make clean
	cd unitests && make clean
