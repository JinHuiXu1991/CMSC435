CC          = c++ 

#-----------------------------------------
#Optimization ----------------------------
OPT   = -O3 -Wno-deprecated

#GL_LIB = -framework OpenGL -framework GLUT -framework foundation
GL_LIB = -lGL -lglut -lGLU

#-----------------------------------------

TARGETS = fishtank 

OBJECTS = fishtank.o slVector.o 

#-----------------------------------------

LIBS = $(GL_LIB) 

CCOPTS = $(OPT) $(INCS) 
LDOPTS = $(OPT) $(INCS) $(LIBS) 

#-----------------------------------------
#-----------------------------------------

default: $(TARGETS)


clean:
	/bin/rm -f *.o $(TARGETS)

#-----------------------------------------
#-----------------------------------------

fishtank: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDOPTS) -o fishtank 

#-----------------------------------------
#-----------------------------------------

.cpp.o: 
	$(CC) $(CCOPTS) -c $< 

#-----------------------------------------
#-----------------------------------------















