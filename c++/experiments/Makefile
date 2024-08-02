all: tests.exe

tests.exe: tests.cpp test_all_01s.cpp test_all_perms.cpp test_common.cpp
	g++ -O3 -o tests.exe tests.cpp test_all_01s.cpp test_all_perms.cpp test_common.cpp

clean:
	rm -f *.o tests.exe
