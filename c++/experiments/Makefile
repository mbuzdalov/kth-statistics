all: tests.exe performance.exe tuning.exe

# also serves to track dependencies on the header-only algorithms
predictors.o: kth_statistic_predictor_simple.cpp kth_statistic.h kth_statistic_stl.h kth_statistic_hoare.h kth_statistic_predictor_simple.h
	g++ -std=c++20 -Wall -Wpedantic -O3 -c -o predictors.o kth_statistic_predictor_simple.cpp

tests.exe: tests.cpp test_all_01s.cpp test_all_perms.cpp test_random.cpp test_common.cpp predictors.o
	g++ -std=c++20 -Wall -Wpedantic -O3 -o tests.exe predictors.o tests.cpp test_all_01s.cpp test_all_perms.cpp test_random.cpp test_common.cpp

performance.exe: performance.cpp predictors.o
	g++ -std=c++20 -Wall -Wpedantic -O3 -o performance.exe predictors.o performance.cpp

tuning.exe: tuning.cpp predictors.o
	g++ -std=c++20 -Wall -Wpedantic -O3 -o tuning.exe predictors.o tuning.cpp

clean:
	rm -f *.o *.exe
