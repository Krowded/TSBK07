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
#include "LoadTGA.h"
#include "VectorUtils3.h"

// Globals
// Data would normally be read from files
Model *m1, *m2,*m3;

mat4 projection, camera;

//Reference to shader program
GLuint program;

//Reference to texture
GLuint texRef;

void init(void)
{

	projection = frustum(-0.8, 0.8, -0.45, 0.45, 1.0, 1000.0);
	camera = lookAt(0, 0, 500, 0, 0, 0, 0, 1 ,0);


	dumpInfo();

	// GL inits
	glClearColor(0.0,0.0,0.0,0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");
	

	//Load models

	m1 = LoadModelPlus("teddy.obj");
	m2 = LoadModelPlus("models/castle/walls.obj");
	m3 = LoadModelPlus("models/castle/rooftops.obj");
	
	// End of upload of geometry

	// Load texture
	LoadTGATextureSimple("maskros512.tga", &texRef);
	//glBindTexture(GL_TEXTURE_2D, texRef);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);

	
	printError("init arrays");
}

GLfloat t = 0;
GLfloat tlast = 0;

void display(void)
{
	mat4 rotation, translation, model, mv, mvp;
	GLfloat tdelta;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	tlast = t;
	t = glutGet(GLUT_ELAPSED_TIME) / 2000.0 ;
	tdelta = t-tlast;

	glUniform1f(glGetUniformLocation(program, "t"), t);

	if(t < 6.29)
	{
		GLfloat v = (camera.m)[11];
		camera = Mult( T(0,0,-v), camera);
		camera = Mult(Ry(tdelta), camera);
		camera = Mult(T(0,0,v), camera);
	}
	if(50*t < 370)
		camera = Mult( T(0, 0, 50*tdelta), camera);
	if(15*t < 120)
		camera = Mult( T(0, 15*tdelta, 0), camera);


	// Model 1
	translation = T(-3.0, -120.0, 60.0);
	rotation = Mult(Ry(2*t),Rx(0));
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m1, program, "inPosition", "inNormal", "inTexCoord");


	// Model 2
	translation = T(0.0, -150.0, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m2, program, "inPosition", "inNormal", "inTexCoord");

	// Model 3
	translation = T(0.0, -150.0, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m3, program, "inPosition", "inNormal", "inTexCoord");

	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(20, &OnTimer, t);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Texture");
	glutReshapeWindow(1024, 576);
	glutDisplayFunc(display); 	
	init ();
	glutMainLoop();
	return 0;
}
