subdivide: slVector.o subdivide.o
	g++ -std=gnu++0x -Wall slVector.o subdivide.o -o subdivide

subdivide.o: subdivide.H subdivide.cpp
	g++ -std=gnu++0x -Wall -c subdivide.cpp

slVector.o: slIO.H slVector.H slVector.cpp
	g++ -std=gnu++0x -Wall -c slVector.cpp


clean:
	rm -f *.o subdivide *~ *.gch

run:
	./subdivide














