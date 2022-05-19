CXX = g++
CC = gcc

SOURCES = matrixf.cpp matrix.cpp netfeedf.cpp netfeedl.cpp netfeedft.cpp netfeedftp.cpp
OBJECTS = $(SOURCES:%.cpp=%.o)
HEADERS = $(SOURCES:%.cpp=%.h)
FLAGS = -std=gnu++11
LIBS = 
EXECUTABLES = 
all: $(OBJECTS) $(EXECUTABLES)
	cd unitests && make

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