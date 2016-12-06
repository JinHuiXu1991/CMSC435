trace: rayTracing.o vector.o readNFF.o trace.o
	g++ -ansi -Wall rayTracing.o vector.o readNFF.o trace.o -o trace

trace.o: trace.cpp
	g++ -ansi -Wall -c trace.cpp

readNFF.o: readNFF.h readNFF.cpp
	g++ -ansi -Wall -c readNFF.cpp

vector.o: vector.h vector.cpp
	g++ -ansi -Wall -c vector.cpp

rayTracing.o: rayTracing.h rayTracing.cpp
	g++ -ansi -Wall -c rayTracing.cpp

clean:
	rm -f *.o trace *~ *.gch

run:
	./trace