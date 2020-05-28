CC=gcc#
CXX=g++#
CXXFLAGS=-std=c++11 -O3 -pthread
CXXFLAGS_TEST=-g -std=c++11 -fPIC -O3 -pthread -fvisibility=hidden -Wall -Wextra
ROOT_DIR=$(shell pwd)

LIB_FLAGS=
INCLUDE_FLAGS=-I$(ROOT_DIR)/include

benchmark: 
	mkdir -p build
	$(CXX) $(CXXFLAGS) benchmarks/$(name)/*.cpp -o build/$(name) $(INCLUDE_FLAGS) $(LIB_FLAGS)

test: test_main test_scalar

test_main:
	mkdir -p tests/build
	$(CXX) $(CXXFLAGS) -c tests/test_main.cpp -o tests/build/main.o

test_scalar:
	cd tests/build && $(CXX) $(CXXFLAGS_TEST) main.o $(ROOT_DIR)/tests/test_scalar.cpp -o test_scalar $(INCLUDE_FLAGS) $(LIB_FLAGS)
	cd tests/build && ./test_scalar -r compact
