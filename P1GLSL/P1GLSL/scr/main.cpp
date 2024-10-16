#include "BOX.h"
#include <IGL/IGlib.h>
#include <Gl/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
using namespace IGlib;
using namespace std;
using namespace glm;


//Idenficadores de los objetos de la escena
int objId = -1;
// añadimos un segundo cubo
int obj2Id = -1;
// añadimos un tercer cubo para bezier
int obj3Id = -1;

//vectores de la camara
vec3 COP = vec3(0.0f, 0.0f, 6.0f);
vec3 LookAt = vec3(0.0f, 0.0f, -1.0f);
vec3 VUP = vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 0.1f;
float rotationSpeed = 0.05f;

//Camara con el raton




//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);


int main(int argc, char** argv)
{
	locale::global(locale("spanish"));// acentos ;)
	if (!init("../shaders_P1/shader.p1.vert", "../shaders_P1/shader.p1.frag"))
		return -1;

	//Se ajusta la cámara
	//Si no se da valor se cojen valores por defecto



	float n = 1.;
	float f = 10.;
	mat4 model = mat4(1.0);
	mat4 view = mat4(1.0);
	mat4 proj = mat4(0.0);

	view[3].z = -6.f;


	proj[0].x = 1.732;
	proj[1].y = 1.732;
	proj[2][2] = (f + n) / (n - f);
	proj[2][3] = -1.f;
	proj[3].z = 2.f * f * n / (n - f);

	setProjMat(proj);
	setViewMat(view);

	//Creamos el objeto que vamos a visualizar
	objId = createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	obj2Id = createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	obj3Id = createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	//una matriz por objeto
	mat4 modelMat = mat4(1.0f);
	mat4 model2Mat = mat4(1.0f);
	mat4 model3Mat = mat4(1.0f);

	setModelMat(objId, modelMat);

	setModelMat(obj2Id, model2Mat);

	//Incluir texturas aquí.

	addColorTex(objId, "../img/gradient.png");

	//CBs
	setResizeCB(resizeFunc);
	setIdleCB(idleFunc);
	setKeyboardCB(keyboardFunc);
	setMouseCB(mouseFunc);
	setMouseMoveCB(mouseMotionFunc);
	glutPassiveMotionFunc(mouseMotionFunc);

	//Mainloop
	mainLoop();
	destroy();
	return 0;
}

// hemos creado un metodo que permite devolver una matriz de rotacion en el eje Y 
glm::mat4 createRotationMatrix(char axis, float angle)
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
mat4 orbitalSpinAxis(char axis, float angle , float orbitRadius) {
	// Matriz identidad inicial
	mat4 model = mat4(1.0f);

	// vector de desplazamiento con el radio de la orbita 

	model[3][0] = orbitRadius;
	model[3][1] = 0.0f;
	model[3][2] = 0.0f;
	// Aplicamos la rotación alrededor del eje Y
	// 'angle' es el ángulo en radianes que queremos rotar
	model = createRotationMatrix(axis, angle) * model;

	return model;



}

vec3 bezierCalc(float t, vec3 P0, vec3 P1, vec3 P2, vec3 P3) {

	


}


mat4 orbitalBezier(char axis, float t) {

	mat4 modelBezier = mat4(1.0f);

	// Puntos de control
	vec3 P0, P1, P2, P3;
	vec3 arrayPuntos[]; // superior a 4 y par (obligatorio)

	switch (axis) {
	case 'X':  
		// Eje X: órbita circular en el plano YZ
		P0 = vec3(0.0f, -orbitRadius, -orbitRadius);
		P1 = vec3(0.0f, -orbitRadius, orbitRadius);
		P2 = vec3(0.0f, orbitRadius, -orbitRadius);
		P3 = vec3(0.0f, orbitRadius, orbitRadius);
		break;
	case 'Y':  // Eje Y: órbita circular en el plano XZ
		P0 = vec3(-orbitRadius, 0.0f, -orbitRadius);
		P1 = vec3(orbitRadius, 0.0f, -orbitRadius);
		P2 = vec3(-orbitRadius, 0.0f, orbitRadius);
		P3 = vec3(orbitRadius, 0.0f, orbitRadius);
		break;
	case 'Z':  // Eje Z: órbita circular en el plano XY
		P0 = vec3(-orbitRadius, -orbitRadius, 0.0f);
		P1 = vec3(orbitRadius, -orbitRadius, 0.0f);
		P2 = vec3(-orbitRadius, orbitRadius, 0.0f);
		P3 = vec3(orbitRadius, orbitRadius, 0.0f);
		break;
	default:
		// Si el eje no es correcto, volvemos al caso por defecto
		P0 = vec3(orbitRadius, 0.0f, 0.0f);
		P1 = vec3(orbitRadius, 0.0f, 0.0f);
		P2 = vec3(orbitRadius, 0.0f, 0.0f);
		P3 = vec3(orbitRadius, 0.0f, 0.0f);
		break;
		
	}
/*
	int tfloored = floor(t);
	float tdecimal = ;
*/
	// precalcular valores de la ecuación de bezier
	float t2 = t * t;
	float t3 = t2 * t;
	float UMT = 1.0f - t;     // Uno Menos T >> UMT
	float UMT2 = UMT * UMT;   // cuadrado UMT
	float UMT3 = UMT2 * UMT;  // cubo UMT

	// Fórmula de la curva Bezier cúbica
	vec3 bezier = (UMT3 * P0) + (3 * UMT2 * t * P1) + (3 * UMT * t2 * P2) + (t3 * P3);

	// Asignamos la posición calculada en la matriz
	modelBezier[3][0] = bezier.x;
	modelBezier[3][1] = bezier.y;
	modelBezier[3][2] = bezier.z;

	
	// Rotación alrededor del eje especificado
	modelBezier = createRotationMatrix(axis, angle) * modelBezier;
	//modelBezier = createRotationMatrix(axis, angle) * modelBezier;

	return modelBezier;


}

mat4 createViewMatrix(vec3 CoP, vec3 LookAt, vec3 VUP){

	vec3 N = normalize(LookAt - CoP);  // Vector que apunta desde CoP hacia LookAt
	vec3 V = normalize(cross(N, VUP));  // Eje derecha
	vec3 U = normalize(cross(V, N));    // Eje arriba ajustado

	// Matriz de rotación de la cámara

	mat4 rotation = mat4(1.0f);
	rotation[0] = vec4(V, 0.0f);  // Eje V como primera fila
	rotation[1] = vec4(U, 0.0f);  // Eje U como segunda fila
	rotation[2] = vec4(-N, 0.0f); // -N como tercera fila (cámara mira en la dirección -Z)


	mat4 traslacion = mat4(1.0f);
	
	traslacion[3][0] = -COP.x;
	traslacion[3][1] = -COP.y;
	traslacion[3][2] = -COP.z; // Traslación inversa según posición CoP

	// Multiplicamos rotación y traslación para obtener la matriz de vista
	return rotation * traslacion;
}


mat4 createProjMatrix(int width, int height, float anguloApertura) {

	// Ajusta el aspect ratio al tamaño de la ventana

	float aspectRatio = (float)width / (float)height;

	float apertura = anguloApertura * (pi<float>() / 180);




	//Mismos calculos de la matriz de proyeccion , pero cambiamos las componentes [0][0] , [1][1] : adaptamos el calculo del ratio

	float n = 1.;
	float f = 10.;

	mat4 proj = mat4(0.0);

	proj[0].x = 1.0f / (aspectRatio * tan(apertura / 2.0f));
	proj[1].y = 1.0f / tan(apertura / 2.0f);
	proj[2][2] = (f + n) / (n - f);
	proj[2][3] = -1.f;
	proj[3].z = 2.f * f * n / (n - f);


	return proj;

}



float clampValue(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}


void resizeFunc(int width, int height)
{
	// usamos nuestro metodo para crear una matriz de proyeccion
	setProjMat(createProjMatrix(width, height, 45.0f));

}

void idleFunc()
{
	static float angle = 0;
	angle = (angle < (pi<float>() * 2)) ? angle + 0.01f : 0.0f;

	// ESTE rot ES EL QUE VIMOS EN CLASE , NO ES ILEGAL
	mat4 rot = rotate(mat4(1.0f), angle, vec3(1.0f, 1.0f, 0.0f));

	mat4 orbitaBezier = orbitalBezier('Z', 0.7f,angle,1.0f);
	// Como rota sobre el eje Y , cambiamos la componentes XZ y dejamos la Y a 0
	mat4 rot2 = createRotationMatrix('Y', angle);
	mat4 rot3 = createRotationMatrix('X', angle);

	mat4 orbita = orbitalSpinAxis('Y', angle , 3.0f);

	

	setModelMat(objId, rot);
	setModelMat(obj2Id, orbita * rot2);
	setModelMat(obj3Id, orbitaBezier * rot3);

}

void updateViewMatrix()
{
	//prohibido usar lookAt
	mat4 view = createViewMatrix(COP, LookAt, VUP);
	setViewMat(view);
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

	updateViewMatrix();
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state == 0)
		cout << "Se ha pulsado el botón ";
	else
		cout << "Se ha soltado el botón ";

	if (button == 0) mouseFunc(x ,y);
	if (button == 1) cout << "central del ratón " << endl;
	if (button == 2) cout << "de la derecha del ratón " << endl;


	std::cout << "en la posición " << x << " " << y << endl << endl;

	cout << "en la posición " << x << " " << y << endl << endl;

	cout << "en la posición " << x << " " << y << endl << endl;
}

void mouseMotionFunc(int x, int y)
{
	// camara orbital
	//rotar los vectores respecto a un punto (miriar con gepeto)

}

