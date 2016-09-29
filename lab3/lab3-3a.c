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

void CameraControl(GLfloat);

// Globals

// Data would normally be read from files
Model *m1, *m2,*m3, *m4, *skym, *mground;

mat4 projection, camera, cameraPosition;


GLfloat groundPolygon[] = 
{
	 1000.0, 0.0, -1000.0,
	-1000.0, 0.0,  1000.0,
	 1000.0, 0.0,  1000.0,

	 1000.0, 0.0, -1000.0,
	-1000.0, 0.0,  1000.0,
	-1000.0, 0.0, -1000.0
};

GLfloat groundTexCoord[] =
{
	1.0, 0.0,
	0.0, 1.0,
	0.0, 0.0,

	1.0, 0.0,
	0.0, 1.0,
	1.0, 1.0
};

GLfloat groundNormal[] =
{
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0
};

//Reference to shader program
GLuint program, programSky;

//Reference to texture
GLuint texRefGround;
GLuint texRefSky;

unsigned int groundVertexArrayObjID;

void init(void)
{
	unsigned int groundVertexBufferObjID;
	unsigned int groundTexCoordBufferObjID;
	unsigned int groundNormalBufferObjID;

	projection = frustum(-0.8, 0.8, -0.45, 0.45, 1.0, 1000.0);
	cameraPosition = lookAt(0, 2, 50, 0, 2, 0, 0, 1, 0);
	camera = cameraPosition;

	dumpInfo();

	// GL inits
	glClearColor(0.0, 1.0, 1.0, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	// Load and compile shader
	programSky = loadShaders("lab3-3sky.vert", "lab3-3sky.frag");
	program = loadShaders("lab3-3.vert", "lab3-3.frag");
	printError("init shader");

	// Load ground
	mground = LoadDataToModel(groundPolygon, groundNormal, groundTexCoord, NULL, NULL, 6, NULL);

	//Load models

	m1 = LoadModelPlus("windmill/windmill-walls.obj");
	m2 = LoadModelPlus("windmill/windmill-roof.obj");
	m3 = LoadModelPlus("windmill/windmill-balcony.obj");
	m4 = LoadModelPlus("windmill/blade.obj");
	skym = LoadModelPlus("skybox.obj");

	// End of upload of geometry

	// Load texture
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texRef);
	LoadTGATextureSimple("dirt.tga", &texRefGround);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);


	//Upload directional light
	float inLight[3] = { 0.58, 0.58, 0.58 };
	glUniform3fv(glGetUniformLocation(program, "inLight"), 1, inLight);
	
	printError("init arrays");
}

void display(void)
{
	mat4 rotation, translation, model, mv, mvp;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 2000.0 ;

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
	
	timeRotation = Mult( T(fulcrum.x, fulcrum.y, fulcrum.z), Mult( ArbRotate(axis, t), T(-fulcrum.x, -fulcrum.y, -fulcrum.z)));



	//Blades
	int i;
	for (i = 0; i < 4; ++i)
	{
		translation = T(4.5, 9.2, 0.0);
		rotation = Mult(Ry(0),Rx((M_PI*0.5)*i));
		model = Mult( Mult(translation, rotation), timeRotation);
		mv = Mult(camera, model);
		mvp = Mult(projection, mv);
		glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);

		DrawModel(m4, program, "inPosition", "inNormal", "inTexCoord");
	}


	//Draw gound
	translation = T(0, 0, -500);
	rotation = Rx(0);
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(program, "mv"), 1, GL_TRUE, mv.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp.m);
	DrawModel(mground, program, "inPosition", "inNormal", "inTexCoord");

	//Draw sky
	glDisable(GL_CULL_FACE);
	glDepthRange(1.0, 1.0);
	translation = T(0, 0, 0);
	rotation = Rx(0);
	model = Mult(translation, rotation);
	mv = Mult(camera, model);
	(mv.m)[3] = 0;
	(mv.m)[7] = 0;
	(mv.m)[11] = 0;
	mvp = Mult(projection, mv);
	glUniformMatrix4fv(glGetUniformLocation(programSky, "mvp"), 1, GL_TRUE, mvp.m);
	DrawModel(skym, programSky, "inPosition", NULL, "inTexCoord");
	glDepthRange(0.0, 1.0);
	glEnable(GL_CULL_FACE);

	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int value)
{
	CameraControl(value);

	printf("hhs");

	printf("OnTimer %d \n", value);

	glutPostRedisplay();
	glutTimerFunc(20, &OnTimer, value);
}

mat4 directions;
vec3 backDirectionVec, rightDirectionVec, upDirectionVec;

void CameraMouseUpdate(int mouseX, int mouseY)
{	
		static GLfloat x = 0;
		static GLfloat y = 0;	

		//Update mousepointer value
		if (mouseY != 0)
		{
			x = (GLfloat)mouseX;

			//Clamp Y
			if (mouseY < 375 && mouseY > 276)
			y = (GLfloat)mouseY;
		}

		camera = cameraPosition;
		camera = Mult(Ry(2*M_PI*x/512), Mult( ArbRotate(rightDirectionVec, 2*M_PI*y/324), camera));		
}

void CameraControl(GLfloat tlast)
{
	GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	printf("muahaha %d \n", t);

	GLfloat speed = 1;

	directions = Transpose(camera);
	backDirectionVec = Normalize(SetVector( (directions.m)[2], (directions.m)[6], (directions.m)[10]));
	rightDirectionVec = Normalize(SetVector( (directions.m)[0], (directions.m)[4], (directions.m)[8]));	
	upDirectionVec = Normalize(SetVector( (directions.m)[1], (directions.m)[5], (directions.m)[9]));

	//Cant move in y direction, y direction removed, speed added
	vec3 backVec = ScalarMult( Normalize(SetVector(backDirectionVec.x, 0, backDirectionVec.z)), speed);
	vec3 rightVec = ScalarMult( Normalize(SetVector(rightDirectionVec.x, 0, rightDirectionVec.z)), speed);
	vec3 upVec = ScalarMult( Normalize(SetVector(upDirectionVec.x, 0, upDirectionVec.z)), speed);
	if (glutMouseIsDown(' '))
		glutPassiveMotionFunc(CameraMouseUpdate);
	if (glutKeyIsDown('w'))
		cameraPosition = Mult( T( backVec.x, backVec.y, backVec.z), cameraPosition);
	if (glutKeyIsDown('a')) 
		cameraPosition = Mult( T( rightVec.x, rightVec.y, rightVec.z), cameraPosition);
	if (glutKeyIsDown('s') )
		cameraPosition = Mult( T( -backVec.x, -backVec.y, -backVec.z), cameraPosition);
	if (glutKeyIsDown('d'))
		cameraPosition = Mult( T( -rightVec.x, -rightVec.y, -rightVec.z), cameraPosition);

	CameraMouseUpdate(0,0);

	glutTimerFunc(20, &CameraControl, t);
}



int main(int argc, char *argv[])
{
	GLfloat tstart = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(20, OnTimer, tstart);
	//glutTimerFunc(20, &CameraControl, tstart);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Texture");
	glutReshapeWindow(1024, 576);
	glutDisplayFunc(display); 

	init();

	tstart = (GLfloat)glutGet(GLUT_ELAPSED_TIME);

	printf("Tstart  %d \n", tstart);

	glutMainLoop();
	return 0;
}


//turbosquid for models