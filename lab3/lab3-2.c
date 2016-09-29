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
Model *m1, *m2,*m3, *m4;

mat4 projection, camera, startCamera;

//Reference to shader program
GLuint program;

//Reference to texture
GLuint texRef;

void init(void)
{

	projection = frustum(-0.8, 0.8, -0.45, 0.45, 1.0, 1000.0);
	startCamera = lookAt(0, 20, 50, 0, 20, 0, 0, 1, 0);
	camera = startCamera;


	dumpInfo();

	// GL inits
	glClearColor(1.0, 1.0, 1.0, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-2.vert", "lab3-2.frag");
	printError("init shader");
	

	//Load models

	m1 = LoadModelPlus("windmill/windmill-walls.obj");
	m2 = LoadModelPlus("windmill/windmill-roof.obj");
	m3 = LoadModelPlus("windmill/windmill-balcony.obj");
	m4 = LoadModelPlus("windmill/blade.obj");

	// End of upload of geometry

	// Load texture
	//LoadTGATextureSimple("maskros512.tga", &texRef);
	//glBindTexture(GL_TEXTURE_2D, texRef);
	//glUniform1i(glGetUniformLocation(program, "texUnit"), 0);

	
	printError("init arrays");
}

GLfloat tnow = 0;
GLfloat tlast = 0;
GLfloat angle = 0;

void display(void)
{
	mat4 rotation, translation, model, mv, mvp;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	tlast = tnow;
	tnow = glutGet(GLUT_ELAPSED_TIME) / 2000.0 ;

	glUniform1f(glGetUniformLocation(program, "t"), tnow);

	// Walls model
	translation = T(0, 0, 0);
	rotation = Mult(Ry(0),Rx(0));
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m1, program, "inPosition", "inNormal", "inTexCoord");


	// Roof model
	translation = T(0.0, 0, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m2, program, "inPosition", "inNormal", "inTexCoord");

	// Balcony model
	translation = T(0.0, 0.0, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m3, program, "inPosition", "inNormal", "inTexCoord");

	//Blades

	//Time rotation
	//Define axis as 2 points:
	vec3 fulcrum, axisPoint, axis;
	mat4 timeRotation;

	fulcrum = SetVector(0, 0, 0);
	axisPoint = SetVector(1, 0, 0);
	axis = VectorSub(axisPoint,fulcrum);
	
	timeRotation = Mult( T(fulcrum.x, fulcrum.y, fulcrum.z), Mult( ArbRotate(axis, tnow), T(-fulcrum.x, -fulcrum.y, -fulcrum.z)));



	//Blade 1
	translation = T(4.5, 9.2, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	model = Mult( Mult(translation, rotation), timeRotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m4, program, "inPosition", "inNormal", "inTexCoord");

	//Blade 2
	translation = T(4.5, 9.2, 0.0);
	rotation = Mult(Ry(0),Rx(3.14/2));
	model = Mult( Mult(translation, rotation), timeRotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m4, program, "inPosition", "inNormal", "inTexCoord");

	//Blade 3
	translation = T(4.5, 9.2, 0.0);
	rotation = Mult(Ry(0),Rx(3.14));
	model = Mult( Mult(translation, rotation), timeRotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m4, program, "inPosition", "inNormal", "inTexCoord");

	//Blade 4
	translation = T(4.5, 9.2, 0.0);
	rotation = Mult(Ry(0),Rx(-3.14/2));
	model = Mult( Mult(translation, rotation), timeRotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

	DrawModel(m4, program, "inPosition", "inNormal", "inTexCoord");
	
	glutSwapBuffers();
}

void CameraMouseUpdate(int mouseX, int mouseY)
{	
		static GLfloat x = 0;
		static GLfloat y = 0;
		static GLfloat cx, cy, cz;

		if (mouseX != 0 || mouseY != 0)
		{
			x = (GLfloat)mouseX;
			y = (GLfloat)mouseY;
		}
		
		camera = startCamera;
		cx = (camera.m)[3];
		cy = (camera.m)[7];
		cz = (camera.m)[11];
		camera = Mult( T(-cx,-cy,-cz), camera);
		camera = Mult(Ry(2*M_PI*x/512),  Mult(Rx(2*M_PI*y/324), camera));
		camera = Mult(T(cx,cy,cz), camera);
		
}

void CameraControl()
{
	if (glutMouseIsDown(' '))
		glutPassiveMotionFunc(CameraMouseUpdate);

	if (glutKeyIsDown('w'))
		startCamera = Mult( T(0, -1, 0), startCamera);
	if (glutKeyIsDown('a')) 
		startCamera = Mult( T(1, 0, 0), startCamera);
	if (glutKeyIsDown('s') )
		startCamera = Mult( T(0, 1, 0), startCamera);
	if (glutKeyIsDown('d'))
		startCamera = Mult( T(-1, 0, 0), startCamera);
	if (glutKeyIsDown('e') )
		startCamera = Mult( T(0, 0, 1), startCamera);
	if (glutKeyIsDown('q'))
		startCamera = Mult( T(0, 0, -1), startCamera);

	CameraMouseUpdate(0,0);
}

void OnTimer(int value)
{
	CameraControl();
	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}

int main(int argc, char *argv[])
{	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Texture");
	glutReshapeWindow(1024, 576);
	glutDisplayFunc(display); 	
	init();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
