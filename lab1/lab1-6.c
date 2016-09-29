// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.
// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include <math.h>
#include "loadobj.h"

#include <stdio.h>
#include <sys/time.h>

// Globals
// Data would normally be read from files

Model *m;

GLuint program;
GLfloat time = 0;



// vertex array object
unsigned int vertexArrayObjID;
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;

void init(void)
{

	m = LoadModel("bunny.obj");

	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program

	printf("saf");
	dumpInfo();
	printf("wfa");

	// GL inits
	glClearColor(0.0,0.0,0.0,0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	printError("GL inits");

	printf("aff");

	// Load and compile shader
	program = loadShaders("lab1-6.vert", "lab1-6.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyVertexBufferObjID);
	glGenBuffers(1, &bunnyIndexBufferObjID);
	glGenBuffers(1, &bunnyNormalBufferObjID);

	glBindVertexArray(bunnyVertexArrayObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

	// VBO for normal data
	glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	// End of upload of geometry
	
	printError("init arrays");
}

void display(void)
{
	printError("pre display");

	GLfloat myTranslationMatrix[] = 
	{    
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};
	
	GLfloat myRotationMatrix3[] = 
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(time/100), sin(time/100), 0.0f,
		0.0f, -sin(time/100), cos(time/100), 0.0f,		
		0.0f, 0.0f, 0.0f, 1.0f 
	};		


	GLfloat myRotationMatrix2[] = 
	{
		cos(time/100), sin(time/100), 0.0f, 0.0f,
		-sin(time/100), cos(time/100), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};		
	
	GLfloat myRotationMatrix[] = 
	{
		cos(time/100), 0.0f, sin(time/100), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(time/100), 0.0f, cos(time/100), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 	
	};

	GLfloat myScalingMatrix[] = 
	{
		time/1000, 0.0f, 0.0f, 0.0f,
		0.0f, time/1000, 0.0f, 0.0f,
		0.0f, 0.0f, time/1000, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f 
	};		
		
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotationMatrix"), 1, GL_TRUE, myRotationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotationMatrix2"), 1, GL_TRUE, myRotationMatrix2);
	glUniformMatrix4fv(glGetUniformLocation(program, "myRotationMatrix3"), 1, GL_TRUE, myRotationMatrix3);
	glUniformMatrix4fv(glGetUniformLocation(program, "myTranslationMatrix"), 1, GL_TRUE, myTranslationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myScalingMatrix"), 1, GL_TRUE, myScalingMatrix);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
    	glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int value)
{

	printf("ontimer %d \n", (int)time);
	time++;	
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);

	printf("ontimer %d \n", (int)time);
}

int main(int argc, char *argv[])
{
	int t = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(20, &OnTimer, t);	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 
	init();	
	glutMainLoop();
	return 0;
}
