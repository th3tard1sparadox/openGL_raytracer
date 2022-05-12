# set this variable to the director in which you saved the common files
commondir = ../common/

all : rayShaders

rayShaders: src/rayShaders.cpp $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -O3 -o rayShaders -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES src/rayShaders.cpp $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm rayShaders
