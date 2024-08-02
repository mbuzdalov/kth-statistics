all: tests.exe performance.exe

tests.exe: tests.cpp test_all_01s.cpp test_all_perms.cpp test_random.cpp test_common.cpp kth_statistic.h kth_statistic_stl.h kth_statistic_hoare.h
	g++ -std=c++20 -Wall -Wpedantic -O3 -o tests.exe tests.cpp test_all_01s.cpp test_all_perms.cpp test_random.cpp test_common.cpp

performance.exe: performance.cpp kth_statistic.h kth_statistic_stl.h kth_statistic_hoare.h
	g++ -std=c++20 -Wall -Wpedantic -O3 -o performance.exe performance.cpp

clean:
	rm -f *.o *.exe
