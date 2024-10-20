#include "BOX.h"
#include <IGL/IGlib.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace std;
using namespace glm;
using namespace IGlib;

//Idenficadores de los objetos de la escena
int objId =-1;











vec3 COP = vec3(0.0f, 0.0f, 6.0f);
vec3 LookAt = vec3(0.0f, 0.0f, -1.0f);
vec3 VUP = vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.1f;
float rotationSpeed = 0.05f;




//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);


mat4 createRotationMatrix(char axis, float angle)
{
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	glm::mat4 rot = glm::mat4(1.0f);  // Matriz identidad

	switch (axis)
	{
	case 'X':  // Rotación en el eje X
		rot[1][1] = cosAngle;  // m[1][1]
		rot[1][2] = -sinAngle; // m[1][2]
		rot[2][1] = sinAngle;  // m[2][1]
		rot[2][2] = cosAngle;  // m[2][2]
		break;

	case 'Y':  // Rotación en el eje Y
		rot[0][0] = cosAngle;  // m[0][0]
		rot[0][2] = sinAngle;  // m[0][2]
		rot[2][0] = -sinAngle; // m[2][0]
		rot[2][2] = cosAngle;  // m[2][2]
		break;

	case 'Z':  // Rotación en el eje Z
		rot[0][0] = cosAngle;  // m[0][0]
		rot[0][1] = -sinAngle; // m[0][1]
		rot[1][0] = sinAngle;  // m[1][0]
		rot[1][1] = cosAngle;  // m[1][1]
		break;

	}

	return rot;
}


int main(int argc, char** argv)
{
	locale::global(locale("spanish"));// acentos ;)
	if (!init("../shaders_P2/shader.v2.vert", "../shaders_P2/shader.v2.frag"))
		return -1;
  //Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto
	mat4 view = mat4(1.0);
	view[3].z = -6;

	mat4 proj = mat4(1.0);
	float f = 1.0f / tan(3.141592f / 6.0f);
	float far = 10.0f;
	float near = 0.1f;

	proj[0].x = f;
	proj[1].y = f;
	proj[2].z = (far + near) / (near - far);
	proj[2].w = -1.0f;
	proj[3].z = (2.0f * far * near) / (near - far);
	proj[3].w = 0.0f;
	IGlib::setProjMat(proj);
	IGlib::setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex, 
			cubeVertexPos, cubeVertexColor, cubeVertexNormal,cubeVertexTexCoord, cubeVertexTangent);

	addColorTex(objId, "../img/color2.png");
	addSpecularTex(objId, "../img/specMap.png");
	addEmissiveTex(objId, "../img/emissive.png");
		
	mat4 modelMat = glm::mat4(1.0f);
	setModelMat(objId, modelMat);
	
	//CBs
	IGlib::setIdleCB(idleFunc);
	setResizeCB(resizeFunc);
	setKeyboardCB(keyboardFunc);
	setMouseCB(mouseFunc);
    setMouseMoveCB(mouseMotionFunc);
	
	//Mainloop
	mainLoop();
	destroy();
	return 0;
}

void resizeFunc(int width, int height)
{
	//Ajusta el aspect ratio al tamaño de la venta
}

void idleFunc()
{
	mat4 modelMat(1.0f);
	static float angle = 0.0f;
	angle = (angle > pi<float>() * 2.0f) ? 0 : angle + 0.01f;
	
	modelMat = rotate(modelMat, angle, vec3(1.0f, 1.0f, 0.0f));

	setModelMat(objId, modelMat);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {

	case 'w':

		COP += cameraSpeed * LookAt;
		break;

	case 's':
		COP -= cameraSpeed * LookAt;
		break;

	case 'a':
		// producto vectorial de los dos vectores de la camara , hallamos el vector correspondiente y lo multiplicamos por la velocidad de la camara
		COP -= normalize(cross(LookAt, VUP)) * cameraSpeed;
		break;

	case 'd':
		// mismo caso que la a , pero en positivo(right)
		COP += normalize(cross(LookAt, VUP)) * cameraSpeed;
		break;

	case 'q':
		// Rotar cámara hacia la izquierda en el eje Y
	{
		// Generamos una matriz de rotación en el eje Y usando createRotationMatrix
		mat4 rotationMat = createRotationMatrix('Y', rotationSpeed);

		// Actualizamos la posición de la cámara COP respecto al punto de interés (LookAt)
		vec3 direction = COP - LookAt;
		direction = vec3(rotationMat * vec4(direction, 0.0f));


		COP = LookAt + direction;  // Actualizamos la nueva posición de la cámara
	}
	break;

	case 'e':
		// Rotar cámara hacia la derecha en el eje Y
	{
		// Generamos una matriz de rotación en el eje Y usando createRotationMatrix con ángulo negativo

		mat4 rotationMat = createRotationMatrix('Y', -rotationSpeed);


		// Actualizamos la posición de la cámara COP respecto al punto de interés (LookAt)
		vec3 direction = COP - LookAt;  // Distancia entre la cámara y el punto que está mirando
		direction = vec3(rotationMat * vec4(direction, 0.0f));  // Rotamos el vector de dirección

		COP = LookAt + direction;  // Actualizamos la nueva posición de la cámara
	}
	break;
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state==0)
		cout << "Se ha pulsado el botón ";
	else
		cout << "Se ha soltado el botón ";
	
	if (button == 0) cout << "de la izquierda del ratón " << endl;
	if (button == 1) cout << "central del ratón " << endl;
	if (button == 2) cout << "de la derecha del ratón " << endl;

	cout << "en la posición " << x << " " << y << endl << endl;
}

void mouseMotionFunc(int x, int y)
{

}
