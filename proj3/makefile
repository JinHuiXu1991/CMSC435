CC          = c++ 

#-----------------------------------------
#Optimization ----------------------------
OPT   = -O3 -Wno-deprecated

# use this next line on osx
#GL_LIB = -framework OpenGL -framework GLUT -framework foundation
GL_LIB = -lGL -lglut -lGLU

#-----------------------------------------

TARGETS = walk

OBJECTS = walk.o slVector.o 

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

walk: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDOPTS) -o walk 

#-----------------------------------------
#-----------------------------------------

.cpp.o: 
	$(CC) $(CCOPTS) -c $< 

#-----------------------------------------
#-----------------------------------------















