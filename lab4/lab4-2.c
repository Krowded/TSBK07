	// Lab 4, terrain generation

	#ifdef __APPLE__
		#include <OpenGL/gl3.h>
		// Linking hint for Lightweight IDE
		// uses framework Cocoa
	#endif
	#include "MicroGlut.h"
	#include "GL_utilities.h"
	#include "VectorUtils3.h"
	#include "loadobj.h"
	#include "LoadTGA.h"
	#include <math.h>

	mat4 camMatrix, camBaseMatrix;

	float TerrainHeight(vec3);

	Model* GenerateTerrain(TextureData *tex)
	{
		int vertexCount = tex->width * tex->height;
		int triangleCount = (tex->width-1) * (tex->height-1) * 2;
		int x, z;
		
		GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
		GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
		GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
		GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);
		
		printf("bpp %d\n", tex->bpp);
		for (x = 0; x < tex->width; x++)
			for (z = 0; z < tex->height; z++)
			{
	// Vertex array. You need to scale this properly
				vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
				vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 10.0;
				vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
	// Texture coordinates. You may want to scale them.
				texCoordArray[(x + z * tex->width)*2 + 0] = x;//(float)x / tex->width;
				texCoordArray[(x + z * tex->width)*2 + 1] = z;//(float)z / tex->height;
			}
		for (x = 0; x < tex->width-1; x++)
			for (z = 0; z < tex->height-1; z++)
			{
			// Triangle 1
				indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
			// Triangle 2
				indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
				indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
			// Normal vectors. You need to calculate these.
				if (x > 0 && x < tex->width-1 && z > 0 && z < tex->height-1 )
				{
					vec3 leftRightVector = SetVector( (vertexArray[(x - 1 + z * tex->width)*3 + 0] - vertexArray[(x + 1 + z * tex->width)*3 + 0]),
													  (vertexArray[(x - 1 + z * tex->width)*3 + 1] - vertexArray[(x + 1 + z * tex->width)*3 + 1]),
													  (vertexArray[(x - 1 + z * tex->width)*3 + 2] - vertexArray[(x + 1 + z * tex->width)*3 + 2]));

					vec3 upDownVector = SetVector( (vertexArray[(x + (z - 1) * tex->width)*3 + 0] - vertexArray[(x + (z + 1) * tex->width)*3 + 0]),
												   (vertexArray[(x + (z - 1) * tex->width)*3 + 1] - vertexArray[(x + (z + 1) * tex->width)*3 + 1]),
												   (vertexArray[(x + (z - 1) * tex->width)*3 + 2] - vertexArray[(x + (z + 1) * tex->width)*3 + 2]));
					vec3 normalVector = (CrossProduct(upDownVector, leftRightVector));
					normalArray[(x + z * tex->width)*3 + 0] = normalVector.x;
					normalArray[(x + z * tex->width)*3 + 1] = normalVector.y;
					normalArray[(x + z * tex->width)*3 + 2] = normalVector.z;
				}
				else
				{
					normalArray[(x + z * tex->width)*3 + 0] = 0.0;
					normalArray[(x + z * tex->width)*3 + 1] = 1.0;
					normalArray[(x + z * tex->width)*3 + 2] = 0.0;
				}
			}
		
		// End of terrain generation
		
		// Create Model and upload to GPU:

		Model* model = LoadDataToModel(
				vertexArray,
				normalArray,
				texCoordArray,
				NULL,
				indexArray,
				vertexCount,
				triangleCount*3);

		return model;
	}


	// vertex array object
	Model *m, *m2, *tm;
	// Reference to shader program
	GLuint program, modelprogram;
	GLuint tex1, tex2;
	TextureData ttex; // terrain

	void init(void)
	{
		// GL inits
		glClearColor(0.2,0.2,0.5,0);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		printError("GL inits");

		vec3 cam = {0, 0, 0};
		vec3 lookAtPoint = {0, 0, -4};
		vec3 upVector = {0.0, 1.0, 0.0};
		mat4 projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000.0);
		camBaseMatrix = lookAt(cam.x, cam.y, cam.z,
				    lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
					upVector.x, upVector.y, upVector.z);
		camMatrix = camBaseMatrix;

		// Load and compile shader
		program = loadShaders("terrain2.vert", "terrain2.frag");
		modelprogram = loadShaders("models.vert", "models.frag");
		glUseProgram(program);
		printError("init shader");
		
		glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
		glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0
		LoadTGATextureSimple("maskros512.tga", &tex1);

		glUseProgram(modelprogram);
		glUniformMatrix4fv(glGetUniformLocation(modelprogram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
		
	// Load terrain data
		
		LoadTGATextureData("fft-terrain.tga", &ttex);
		tm = GenerateTerrain(&ttex);
		printError("init terrain");


	// Load models
		m = LoadModelPlus("groundsphere.obj");
		m2 = LoadModelPlus("octagon.obj");
	}

	void display(void)
	{
		static int t = 0;
		++t;
		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		mat4 total, modelView;
		
		printError("pre display");
		

		//Draw terrain
		glUseProgram(program);
		
		modelView = IdentityMatrix();
		total = Mult(camMatrix, modelView);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
		
		glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
		DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");


		//Draw models
		glUseProgram(modelprogram);

		modelView = IdentityMatrix();
		modelView = Mult( T(25*t*0.001, 0, 25*t*0.001), modelView);
		
		static float lastHeight = 0;
		vec3 currentPosition = SetVector((modelView.m)[3], (modelView.m)[7], (modelView.m)[11]); 

		float height = TerrainHeight(currentPosition);
		float heightDifference = height - lastHeight;
		(modelView.m)[7] = lastHeight + heightDifference;
		lastHeight = (modelView.m)[7];

		

		total = Mult(camMatrix, modelView);
		glUniformMatrix4fv(glGetUniformLocation(modelprogram, "mdlMatrix"), 1, GL_TRUE, total.m);
		DrawModel(m, modelprogram, "inPosition", "inNormal", "inTexCoord");

		modelView = IdentityMatrix();
		total = Mult(camMatrix, modelView);
		//total = Mult(S(10, 10, 10), total);
		glUniformMatrix4fv(glGetUniformLocation(modelprogram, "mdlMatrix"), 1, GL_TRUE, total.m);
		DrawModel(m2, modelprogram, "inPosition", "inNormal", "inTexCoord");



		printError("display 2");
		
		glutSwapBuffers();
	}

	void CameraControl(int);

	void timer(int t)
	{
		CameraControl(glutGet(GLUT_ELAPSED_TIME));
		glutPostRedisplay();
		glutTimerFunc(10, &timer, 0);
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

			camMatrix = Mult(Ry(2*M_PI*x/512), Mult( ArbRotate(rightDirectionVec, 2*M_PI*y/324), camBaseMatrix));		
	}


	float TerrainHeight(vec3 currentPosition)
	{
		int x,z;
		//Find the quad
		currentPosition.x = currentPosition.x;
		currentPosition.z = currentPosition.z;
		float x1 = floor(currentPosition.x);
		float x2 = floor(currentPosition.x + 1);

		float z1 = floor(currentPosition.z);
		float z2 = floor(currentPosition.z + 1);

		//Determine the triangle
		if( x1 < 0 || z1 < 0 || x2 > ttex.width || z2 > ttex.height ) //Outside texture
		{
			return -100;
		}
		else if( (x2 - currentPosition.x) > (currentPosition.z - z1)) //Left triangle
		{
			x = (int)x1;
			z = (int)z1;

		}
		else //Right triangle
		{
			x = (int)x2;
			z = (int)z2;
		}

		vec3 startPoint = SetVector(tm->vertexArray[(x + z * ttex.width)*3 + 0], 
			                        tm->vertexArray[(x + z * ttex.width)*3 + 1], 
			                        tm->vertexArray[(x + z * ttex.width)*3 + 2]);

		vec3 vectorX = SetVector( tm->vertexArray[((int)x2 + (int)z1 * ttex.width)*3 + 0],
								  tm->vertexArray[((int)x2 + (int)z1 * ttex.width)*3 + 1],
								  tm->vertexArray[((int)x2 + (int)z1 * ttex.width)*3 + 2]);
		vectorX = VectorSub(vectorX, startPoint);

		vec3 vectorZ = SetVector( tm->vertexArray[((int)x1 + (int)z2 * ttex.width)*3 + 0],
								  tm->vertexArray[((int)x1 + (int)z2 * ttex.width)*3 + 1],
								  tm->vertexArray[((int)x1 + (int)z2 * ttex.width)*3 + 2]);
		vectorZ = VectorSub(vectorZ, startPoint);

		vec3 normal = Normalize(CrossProduct(vectorZ, vectorX));

		return (DotProduct(normal, startPoint) - normal.x * currentPosition.x - normal.z * currentPosition.z) / normal.y;
	}

	int tlast = 0;
	void CameraControl(int t)
	{
		int passedTime = t - tlast;
		tlast = t;	
		GLfloat averageSpeed = 0.02;
		GLfloat speed = (GLfloat)passedTime * averageSpeed;

		directions = Transpose(camMatrix); //Taking the inverse, kinda pointless
		backDirectionVec = Normalize(SetVector( (directions.m)[2], (directions.m)[6], (directions.m)[10]));
		rightDirectionVec = Normalize(SetVector( (directions.m)[0], (directions.m)[4], (directions.m)[8]));	
		upDirectionVec = Normalize(SetVector( (directions.m)[1], (directions.m)[5], (directions.m)[9]));

		//Cant move in y direction, y direction removed, speed added
		vec3 backVec = ScalarMult( Normalize(SetVector(backDirectionVec.x, 0, backDirectionVec.z)), speed);
		vec3 rightVec = ScalarMult( Normalize(SetVector(rightDirectionVec.x, 0, rightDirectionVec.z)), speed);
		vec3 upVec = ScalarMult( Normalize(SetVector(upDirectionVec.x, 0, upDirectionVec.z)), speed);
		if (glutKeyIsDown('w'))
			camBaseMatrix = Mult( T( backVec.x, backVec.y, backVec.z), camBaseMatrix);
		if (glutKeyIsDown('a')) 
			camBaseMatrix = Mult( T( rightVec.x, rightVec.y, rightVec.z), camBaseMatrix);
		if (glutKeyIsDown('s') )
			camBaseMatrix = Mult( T( -backVec.x, -backVec.y, -backVec.z), camBaseMatrix);
		if (glutKeyIsDown('d'))
			camBaseMatrix = Mult( T( -rightVec.x, -rightVec.y, -rightVec.z), camBaseMatrix);
		if (glutKeyIsDown('q'))
			camBaseMatrix = Mult( T( 0, 1, 0), camBaseMatrix);
		if (glutKeyIsDown('e'))
			camBaseMatrix = Mult( T( 0, -1, 0), camBaseMatrix);

		
		//Adjust height to map

		vec3 currentPosition = SetVector(-(camBaseMatrix.m)[3], -(camBaseMatrix.m)[7], -(camBaseMatrix.m)[11]);
		float height = -TerrainHeight(currentPosition) - 5;
		if (height < 95)
		{
			float heightDifference = (camBaseMatrix.m)[7] - height;
			(camBaseMatrix.m)[7] = (camBaseMatrix.m)[7] - heightDifference;
		}

		//Pass it off to update camMatrix
		CameraMouseUpdate(0,0);
	}



	int main(int argc, char **argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
		glutInitContextVersion(3, 2);
		glutInitWindowSize (1024, 576);
		glutCreateWindow ("TSBK07 Lab 4");
		glutDisplayFunc(display);
		init ();
		glutTimerFunc(20, &timer, 0);

		glutPassiveMotionFunc(CameraMouseUpdate);

		glutMainLoop();
		exit(0);
	}
