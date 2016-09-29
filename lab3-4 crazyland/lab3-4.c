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

void CameraControl();
// Globals

// Models
Model *m1, *m2,*m3, *m4, *skym, *groundm;


//Camera matrices
mat4 ViewToProjection, WorldToView, cameraPosition;

//Reference to shader programs
GLuint program, programSky;

//Reference to textures
GLuint texRefGround;
GLuint texRefSky;
GLuint texRefObject;

unsigned int groundVertexArrayObjID;



void UploadLightSources()
{
	Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                                		{0.0f, 1.0f, 0.0f}, // Green light
                                 		{0.0f, 0.0f, 1.0f}, // Blue light
                                 		{1.0f, 1.0f, 1.0f} }; // White light

	GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
	GLint isDirectional[] = {0,0,1,1};

	Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                                      			  {0.0f, 5.0f, 10.0f}, // Green light, positional
                                       			  {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                      			  {0.0f, 0.0f, -1.0f} }; // White light along Z

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirectionsPositionsArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

	printError("UploadLightSources");
}

void LoadModels()
{
	m1 = LoadModelPlus("windmill/windmill-walls.obj");
	m2 = LoadModelPlus("windmill/windmill-roof.obj");
	m3 = LoadModelPlus("windmill/windmill-balcony.obj");
	m4 = LoadModelPlus("windmill/blade.obj");
	skym = LoadModelPlus("skybox.obj");

	printError("LoadModels");
}

void LoadGround()
{
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

	GLuint groundIndices[] =
	{
		1, 2, 3,
		1, 2, 4
	};


	unsigned int groundVertexBufferObjID;
	unsigned int groundTexCoordBufferObjID;
	unsigned int groundNormalBufferObjID;

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &groundVertexArrayObjID);
	glBindVertexArray(groundVertexArrayObjID);

	// Allocate Vertex Buffer Objects
	glGenBuffers(1, &groundVertexBufferObjID);
	
	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, groundVertexBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundPolygon), groundPolygon, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 

	glGenBuffers(1, &groundTexCoordBufferObjID);

	glBindBuffer(GL_ARRAY_BUFFER, groundTexCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundTexCoord), groundTexCoord, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);



	glGenBuffers(1, &groundNormalBufferObjID);

	glBindBuffer(GL_ARRAY_BUFFER, groundTexCoordBufferObjID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundNormal), groundNormal, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);


//	groundm = LoadDataToModel(groundPolygon, groundNormal, groundTexCoord, NULL, groundIndices, 4, 2);
	printError("LoadGround");	
}

void LoadTextures()
{
	LoadTGATextureSimple("dirt.tga", &texRefGround);
	LoadTGATextureSimple("SkyBox512.tga", &texRefSky);
	LoadTGATextureSimple("grass.tga", &texRefObject);

	printError("LoadTextures");
}

void CameraInit()
{
	ViewToProjection = frustum(-0.8, 0.8, -0.45, 0.45, 1.0, 1000.0);
	cameraPosition = lookAt(0, 2, 50, 0, 2, 0, 0, 1, 0);
	WorldToView = cameraPosition;

	printError("CameraInit");
}

void LoadShaders()
{
	// Load and compile shaders
	programSky = loadShaders("lab3-4sky.vert", "lab3-4sky.frag");
	program = loadShaders("lab3-4.vert", "lab3-4.frag");
	printError("LoadShaders");
}

void init(void)
{
	dumpInfo();

	// GL inits
	glClearColor(0.0, 1.0, 1.0, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	printError("GL inits");

	CameraInit();

	LoadShaders();

	LoadModels();
	LoadGround();
	LoadTextures();

	UploadLightSources();

	printError("init");
}

mat4 rotation, translation, ModelToWorld, ModelToView, WorldToProjection, ModelToProjection;

void DrawSky()
{
	glUseProgram(programSky);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	translation = T(0, 0, 0);
	rotation = Rx(0);
	ModelToWorld = Mult(translation, rotation);
	ModelToView = Mult(WorldToView, ModelToWorld);
	(ModelToView.m)[3] = 0;
	(ModelToView.m)[7] = 0;
	(ModelToView.m)[11] = 0;
	ModelToProjection = Mult(ViewToProjection, ModelToView);
	glUniformMatrix4fv(glGetUniformLocation(programSky, "ModelToProjection"), 1, GL_TRUE, ModelToProjection.m);
	glBindTexture(GL_TEXTURE_2D, texRefSky);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	DrawModel(skym, programSky, "inPosition", NULL, "inTexCoord");
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void DrawGround()
{
	glUseProgram(program);

	translation = T(0, 0, -500);
	rotation = Rx(0);
	ModelToWorld = Mult(translation, rotation);
	ModelToView = Mult(WorldToView, ModelToWorld);
	ModelToProjection = Mult(ViewToProjection, ModelToView);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToView"), 1, GL_TRUE, ModelToView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToProjection"), 1, GL_TRUE, ModelToProjection.m);
	glBindTexture(GL_TEXTURE_2D, texRefGround);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindVertexArray(groundVertexArrayObjID);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, 2*3);	// draw object
	
	//DrawModel(groundm, program, "inPosition", "inNormal", "inTexCoord");
}

void DrawModels(GLfloat t)
{
	glUseProgram(program);

	///Windmill

	// Walls model
	translation = T(0.0, 0.0, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	ModelToWorld = Mult(translation, rotation);
	ModelToProjection = Mult(WorldToProjection, ModelToWorld);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToView"), 1, GL_TRUE, ModelToView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToProjection"), 1, GL_TRUE, ModelToProjection.m);

	DrawModel(m1, program, "inPosition", "inNormal", "inTexCoord");


	// Roof model
	translation = T(0.0, 0.0, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	ModelToWorld = Mult(translation, rotation);
	ModelToProjection = Mult(WorldToProjection, ModelToWorld);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToView"), 1, GL_TRUE, ModelToView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToProjection"), 1, GL_TRUE, ModelToProjection.m);

	DrawModel(m2, program, "inPosition", "inNormal", "inTexCoord");

	// Balcony model
	translation = T(0.0, 0.0, 0.0);
	rotation = Mult(Ry(0),Rx(0));
	ModelToWorld = Mult(translation, rotation);
	ModelToProjection = Mult(WorldToProjection, ModelToWorld);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToView"), 1, GL_TRUE, ModelToView.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelToProjection"), 1, GL_TRUE, ModelToProjection.m);

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

	int i;
	for (i = 0; i < 4; ++i)
	{
		translation = T(4.5, 9.2, 0.0);
		rotation = Mult(Ry(0),Rx((M_PI*0.5)*i));
		ModelToWorld = Mult( Mult(translation, rotation), timeRotation);
		ModelToProjection = Mult(WorldToProjection, ModelToWorld);
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelToView"), 1, GL_TRUE, ModelToView.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelToProjection"), 1, GL_TRUE, ModelToProjection.m);
		
		glBindTexture(GL_TEXTURE_2D, texRefObject);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);


		DrawModel(m4, program, "inPosition", "inNormal", "inTexCoord");
	}
	//Other model
}

void display(void)
{
	printError("pre display");

	GLfloat t = glutGet(GLUT_ELAPSED_TIME) / 2000.0;


	Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 1000-200*t}, // Red light, positional
                                      			  {0.0f, 5.0f, 10.0f}, // Green light, positional
                                       			  {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                      			  {0.0f, 0.0f, -1.0f} }; // White light along Z

    //fprintf(stderr, "%f\n", 1000-200*t); 
    
    vec3 campos = SetVector((cameraPosition.m)[3], (cameraPosition.m)[7], (cameraPosition.m)[11]);
    glUniform3fv(glGetUniformLocation(program, "campos"), 1, &campos);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirectionsPositionsArr"), 4, &lightSourcesDirectionsPositions[0].x);

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Upload camera to shader	
	glUniformMatrix4fv(glGetUniformLocation(program, "WorldToView"), 1, GL_TRUE, WorldToView.m);

	WorldToProjection = Mult(ViewToProjection, WorldToView);

	DrawSky();
	DrawGround();
	DrawModels(t);

	printError("display");
	
	glutSwapBuffers();
}

void OnTimer(int t)
{
	CameraControl(glutGet(GLUT_ELAPSED_TIME));
	glutPostRedisplay();
	glutTimerFunc(10, &OnTimer, 0);
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

		WorldToView = Mult(Ry(2*M_PI*x/512), Mult( ArbRotate(rightDirectionVec, 2*M_PI*y/324), cameraPosition));		
}

int tlast = 0;
void CameraControl(int t)
{
	int passedTime = t - tlast;
	tlast = t;
	GLfloat averageSpeed = 0.05;
	GLfloat speed = (GLfloat)passedTime * averageSpeed;

	directions = Transpose(WorldToView);
	backDirectionVec = Normalize(SetVector( (directions.m)[2], (directions.m)[6], (directions.m)[10]));
	rightDirectionVec = Normalize(SetVector( (directions.m)[0], (directions.m)[4], (directions.m)[8]));	
	upDirectionVec = Normalize(SetVector( (directions.m)[1], (directions.m)[5], (directions.m)[9]));

	//Cant move in y direction, y direction removed, speed added
	vec3 backVec = ScalarMult( Normalize(SetVector(backDirectionVec.x, 0, backDirectionVec.z)), speed);
	vec3 rightVec = ScalarMult( Normalize(SetVector(rightDirectionVec.x, 0, rightDirectionVec.z)), speed);
	vec3 upVec = ScalarMult( Normalize(SetVector(upDirectionVec.x, 0, upDirectionVec.z)), speed);
	if (glutKeyIsDown('w'))
		cameraPosition = Mult( T( backVec.x, backVec.y, backVec.z), cameraPosition);
	if (glutKeyIsDown('a')) 
		cameraPosition = Mult( T( rightVec.x, rightVec.y, rightVec.z), cameraPosition);
	if (glutKeyIsDown('s') )
		cameraPosition = Mult( T( -backVec.x, -backVec.y, -backVec.z), cameraPosition);
	if (glutKeyIsDown('d'))
		cameraPosition = Mult( T( -rightVec.x, -rightVec.y, -rightVec.z), cameraPosition);
	if (glutKeyIsDown('q'))
		cameraPosition = Mult( T( 0, 1, 0), cameraPosition);
	if (glutKeyIsDown('e'))
		cameraPosition = Mult( T( 0, -1, 0), cameraPosition);


	CameraMouseUpdate(0,0);
}



int main(int argc, char *argv[])
{	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Texture");
	glutReshapeWindow(1024, 576);
	glutDisplayFunc(display); 
	OnTimer(0);
	init();
	glutPassiveMotionFunc(CameraMouseUpdate);

	glutMainLoop();
	return 0;
}


//turbosquid for models