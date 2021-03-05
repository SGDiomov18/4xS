all: schooling

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

schooling: Makefile src/main.cpp
	g++ -Wall -o schooling src/main.cpp src/Team.cpp src/School.cpp src/Teacher.cpp src/Student.cpp

clean:
	rm -f schooling

run:
	./schooling

fresh:
	rm -f schooling &&\
	rm -f teams.csv &&\
	rm -f students.csv &&\
	g++ -Wall -o schooling src/main.cpp src/Team.cpp src/School.cpp src/Teacher.cpp src/Student.cpp
