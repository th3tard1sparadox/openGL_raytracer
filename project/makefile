# set this variable to the director in which you saved the common files
commondir = ../common/

all : rayShaders

raytracing: raytracing.cpp $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -Wall -O3 -o raytracing -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES raytracing.cpp $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

rayShaders: rayShaders.cpp $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -Wall -O3 -o rayShaders -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES rayShaders.cpp $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm raytracing
	rm rayShaders
