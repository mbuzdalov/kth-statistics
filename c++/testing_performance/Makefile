all: all_benchmarks.exe
	./all_benchmarks.exe

all_benchmarks.exe: all_benchmarks.cpp $(shell find . -iname '*.hpp') $(shell find ../include)
	g++ -I../include -std=c++23 -Wall -Wpedantic -O3 -fconcepts-diagnostics-depth=3 -o all_benchmarks.exe all_benchmarks.cpp

clean:
	rm -f *.o *.exe
