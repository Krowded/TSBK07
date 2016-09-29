# set this variable to the director in which you saved the common files
commondir = ../common/

all : lab3-4

lab3-4 : lab3-4.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab3-4 -I$(commondir) -I../common/Linux lab3-4.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -DGL_GLEXT_PROTOTYPES -lm

clean :
	rm lab3-4
