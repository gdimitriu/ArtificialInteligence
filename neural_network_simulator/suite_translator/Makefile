CXX = g++
SOURCES = 
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS =  $(SOURCES:%.cpp=%.h)
EXECUTABLES = $(OBJECTS) suite_translator

FLAGS = -std=gnu++14 -ggdb -I../include

all: $(EXECUTABLES)

suite_translator: suite_translator.cpp ../include/matrixf.h ../matrixf.cpp ../matrix.cpp
	$(CXX) $(FLAGS) -o suite_translator suite_translator.cpp ../matrixf.cpp ../matrix.cpp

clean:
	for object in $(EXECUTABLES);\
	do \
		if [ -f $$object ]; then rm $$object; fi; \
	done