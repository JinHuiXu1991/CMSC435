stub: CImg.h slVector.o stub.o
	g++ -ansi -Wall CImg.h slVector.o stub.o -o stub

stub.o: stub.cpp
	g++ -ansi -Wall -c stub.cpp

slVector.o: slIO.H slVector.H slVector.cpp
	g++ -ansi -Wall -c slVector.cpp


clean:
	rm -f *.o stub *~ *.gch

run:
	./stub
