all: all_tests.exe
	./all_tests.exe

all_tests.exe: all_tests.cpp $(shell find . -iname '*.hpp') $(shell find ../include)
	g++ -I../include -std=c++23 -Wall -Wpedantic -O3 -fconcepts-diagnostics-depth=3 -o all_tests.exe all_tests.cpp

clean:
	rm -f *.o *.exe
