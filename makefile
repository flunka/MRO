dihtering.exe: dihtering.o TPGM.o
	g++ -g dihtering.o TPGM.o `pkg-config --cflags --libs opencv` -o dihtering.exe


dihtering.o: dihtering.cpp
	g++ -ggdb -g -c dihtering.cpp `pkg-config --cflags --libs opencv`

TPGM.o: TPGM.cpp
	g++ -g -c TPGM.cpp

