# set this variable to the director in which you saved the common files
commondir = ../common/

all : raytracing

raytracing: raytracing.cpp $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	g++ -Wall -o raytracing -I$(commondir) -I../common/Linux -DGL_GLEXT_PROTOTYPES raytracing.cpp $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm raytracing
