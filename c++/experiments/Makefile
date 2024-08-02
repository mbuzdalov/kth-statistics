all: tests.exe

tests.exe: tests.cpp test_all_01s.cpp test_all_perms.cpp
	g++ -O3 -o tests.exe tests.cpp test_all_01s.cpp test_all_perms.cpp

clean:
	rm -f *.o tests.exe
