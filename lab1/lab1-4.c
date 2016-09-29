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

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f,-0.5f,0.5f,
	-0.5f,0.5f,0.0f,
	0.0f,-0.5f,0.0f
};

GLfloat colors[] =
{
	1.0f,0.0f,0.0f,
	0.5f,0.5f,0.0f,
	0.0f,0.5f,1.0f
};

GLfloat myTranslationMatrix[] = 
{    
	1.0f, 0.0f, 0.0f, -0.5f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f 
};



GLuint program;
GLfloat time = 0;



// vertex array object
unsigned int vertexArrayObjID;

void init(void)
{
	// vertex buffer object, used for uploading the geometry
	unsigned int vertexBufferObjID;
	// Reference to shader program
	// GLuint program;

	dumpInfo();

	// GL inits
	glClearColor(0.2,1.2,0.5,0);
	glDisable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab1-4.vert", "lab1-4.frag");
	printError("init shader");
	
	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &vertexArrayObjID);
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	
	// Activate Vertex Array Object
	glBindVertexArray(vertexArrayObjID);
	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "in_Color"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "in_Color"));
	
	

	// End of upload of geometry
	
	printError("init arrays");
}

void display(void)
{
	printError("pre display");

	GLfloat myRotationMatrix[] = 
	{
		cos(time/100), sin(time/100), 0.0f, 0.0f,
		-sin(time/100), cos(time/100), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
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
	glUniformMatrix4fv(glGetUniformLocation(program, "myTranslationMatrix"), 1, GL_TRUE, myTranslationMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "myScalingMatrix"), 1, GL_TRUE, myScalingMatrix);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);	// draw object
	
	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int value)
{
	time++;	
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(20, &OnTimer, t);

	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display); 	
	init ();	
	glutMainLoop();
	return 0;
}
