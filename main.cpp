#include "BOX.h"
#include "auxiliar.h"
#include "shader.h"

#include <windows.h>

#include <gl/glew.h>
#include <gl/gl.h>//Se puede eliminar, se importa opengl en glew
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	proj = glm::mat4(1.0f);
glm::mat4	view = glm::mat4(1.0f);
glm::mat4	model = glm::mat4(1.0f);

//matriz para el giro del segundo cubo
glm::mat4 model2 = glm::mat4(1.0f);
//Luces
glm::vec3 intensidad = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 luzPos = glm::vec3(-3.0f, 0.0f, 0.0f);

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
//Por definir

//VAO VertexArray
unsigned int vao; //Enlaza datos con variables del shader
//VBO VertexBuffer
unsigned int posVBO; //Identeficador de posiciones consecutivas
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;

//Identificadores de textura
unsigned int colorTexId;   //Permite configurar la textura
unsigned int emiTexId;    //Permite configurar la textura

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

//Declaración de CB
void renderFunc(); //Renderizado
void resizeFunc(int width, int height); 
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();

void initObj();
void destroy();


//Carga el shader indicado, devuele el ID del shader
//!Por implementar


//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
//!!Por implementar
unsigned int loadTex(const char *fileName);

glm::mat4 translateFather = glm::mat4(1.0f);
shader obj1;
shader obj2;
int main(int argc, char** argv)
{
	
	std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	/**/
	obj1.initShader("../shaders_P3/shader.v1.vert", "../shaders_P3/shader.v1.frag");
	initObj();
	//Se puede inicializar el cauce despues de iniciar los shaders*/
	obj2.initShader("../shaders_P3/shader.v0.vert", "../shaders_P3/shader.v0.frag");
	initObj();

	glutMainLoop();//Bucle de eventos loop. Inicia idle, sigue render
	destroy();

	return 0;
}
	
//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){
	//Creamos contexto de OPENGL
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//Definimos frame buffer y creamos ventana
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Practicas OPENGL");

	//Inicializamos extensiones
	glewExperimental = GL_TRUE; //La version de glew es antigua. Activamos extension no testeadas
	GLenum error = glewInit();
	if (GLEW_OK != error) {
		std::cout << "Error:" << glewGetErrorString(error) << std::endl;
		exit(-1);
	}
	const GLubyte *oglversion = glGetString(GL_VERSION);
	std::cout << "Este sistema soporta la version de OPENGL: " << oglversion << std::endl;

	//Indica las funciones que trataran eventos
	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);

}

//Configuracion inicial del cauce
void initOGL(){
	glEnable(GL_DEPTH_TEST);//Activamos test de profundidad
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);//Establecemos color de fondo

	glFrontFace(GL_CCW);//Interpretar cual es la cara frontal. CCW=Counter Clock Wise
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//Como rasteriza los triangulos (rellena toda la superficie en este caso)
	glEnable(GL_CULL_FACE);//Activamos la etapa de culling

	proj = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 50.0f); //No tienen que ver con los shaders
	view = glm::mat4(1.0f);
	view[3].z = -6;


}
void destroy(){
	
	//Borramos los VBO y el VAO
	if (sizeof(cubeVertexPos)!= 1) {
		glDeleteBuffers(1, &posVBO);
	}
	if (sizeof(cubeVertexColor) != 1) {
		glDeleteBuffers(1, &colorVBO);
	}
	if (sizeof(cubeVertexNormal) != 1) {
		glDeleteBuffers(1, &normalVBO);
	}
	if (sizeof(cubeVertexTexCoord) != 1) {
		glDeleteBuffers(1, &texCoordVBO);
	}
	glDeleteBuffers(1, &triangleIndexVBO);
	
	glDeleteVertexArrays(1, &vao);
}

//Creamos el programa enlazandolos


void initObj(){
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Si no vale -1, subo los datos
	if (sizeof(cubeVertexPos)!=-1) { 
		//inPos=0
		glGenBuffers(1, &posVBO); //Generamos el vbo de posiciones
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);//Activamos VBO
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexPos, GL_STATIC_DRAW);//NO MODIFICA EL VAO.Al array buffer que este activo, se le sube un conjunto de bytes. No lo modifico y lo dibujo.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //El VBO activo posVBO le asignamos (inPos), vector de 3 coordenadas, en float,no lo normaliza, empieza en 0 y se mueve de 0
		glEnableVertexAttribArray(0); //Habilita el enlace
	} if (sizeof(cubeVertexColor)!= -1) {
		//inColor=1
		glGenBuffers(1, &colorVBO); 
		glBindBuffer(GL_ARRAY_BUFFER, colorVBO); 
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexColor, GL_STATIC_DRAW); 
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(1); 
	} if (sizeof(cubeVertexNormal)!=-1) { 
		//inNormal=2
		glGenBuffers(1, &normalVBO); 
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO); 
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3, cubeVertexNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2); 
	} if (sizeof(cubeVertexTexCoord)!=-1) { 
		//inTexCoord=3
		glGenBuffers(1, &texCoordVBO); 
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO); 
		glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 2, cubeVertexTexCoord, GL_STATIC_DRAW); 
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0); 
		glEnableVertexAttribArray(3); 
	} 
	
	
	//Generamos la lista de indices
	glGenBuffers(1, &triangleIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex, GL_STATIC_DRAW);
	
	colorTexId= loadTex("../img/color2.png");
	emiTexId = loadTex("../img/emissive.png");
	//Iniciamos matriz model otra vez
	model = glm::mat4(1.0f);
	model2 = glm::mat4(1.0f);
}


unsigned int loadTex(const char *fileName){ 
	unsigned char *map;
	unsigned int w, h;
	map = loadTexture(fileName, w, h);
	if(!map){
		std::cout << "Error cargando el fichero" << fileName << std::endl;
		exit(-1);
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);//Activo como textura 2D
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)map);//textura mutable, si fuera textureStore, inmutable
	//A la textura 2D activa, al nivel 0, almacena como rgba8(8 bytes),ancho y alto de textura, border(0), canales del buffer del fichero, como byte sin signo, devuelvo puntero para nota
	delete[]map;//Liberamos memoria borrando map
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//cuando tengo menos fragmentos, 2 acceso a mipmap, 3 acceso a como se combinan
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // cuando tengo muchos fragmentos
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);// cuando la textura sobresale por la coordenada X
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);// cuando la textura sobresale por la coordenada Y
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GL_LINEAR);
	}

	return texId; 
}

void renderFunc(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Limpia los buffers de color y profundidad
	obj1.activateProgram(); //Activamos el programa del objeto
	glm::mat4 modelView = view*model;
	glm::mat4 modelViewProj = proj*modelView;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));
	

	if (obj1.getUModelViewMat() != -1) {
		glUniformMatrix4fv(obj1.getUModelViewMat(), 1, GL_FALSE, &(modelView[0][0]));
	}
	if (obj1.getUModelViewProjMat() != -1) {
		glUniformMatrix4fv(obj1.getUModelViewProjMat(), 1, GL_FALSE, &(modelViewProj[0][0]));//identificador,1 una matriz, no transponer matriz (GL_FALSE),desde que punto comienza a copiar
	}
	if (obj1.getUNormalMat() != -1) {
		glUniformMatrix4fv(obj1.getUNormalMat(), 1, GL_FALSE, &(normal[0][0]));
	}
	if(obj1.getUModelMat() !=-1){
		glUniformMatrix4fv(obj1.getUModelMat(), 1, GL_FALSE, &(model[0][0]));
	}
	if (obj1.getUViewMat() != -1) {
		glUniformMatrix4fv(obj1.getUViewMat(), 1, GL_FALSE, &(view[0][0]));
	}
	if (obj1.getUIntensidad() != -1) {
		glUniform3fv(obj1.getUIntensidad(), 1, &(intensidad[0]));
	}
	if (obj1.getUPosLuz()!= -1) {
		glUniform3fv(obj1.getUPosLuz(), 1, &(luzPos[0]));
	}
	glBindVertexArray(vao);

	if (obj1.getUColorTex() != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(obj1.getUColorTex(), 0);
	}

	if (obj1.getUEmiTex() != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		
		glUniform1i(obj1.getUEmiTex(), 1);
	}
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);//Pinta triangulos, que coja el numero de triangulos por 3, que son de tipo uint y empieza a coger elementos en la posicion 0 del array

	//--------------------------------------------Activamos el 2º objeto
	obj2.activateProgram();
	modelView = view*model2;
	 modelViewProj = proj*modelView;
	normal = glm::transpose(glm::inverse(modelView));
	if (obj2.getUModelViewMat() != -1) {
		glUniformMatrix4fv(obj2.getUModelViewMat(), 1, GL_FALSE, &(modelView[0][0]));
	}
	if (obj2.getUModelViewProjMat() != -1) {
		glUniformMatrix4fv(obj2.getUModelViewProjMat(), 1, GL_FALSE, &(modelViewProj[0][0]));//identificador,1 una matriz, no transponer matriz (GL_FALSE),desde que punto comienza a copiar
	}
	if (obj2.getUNormalMat() != -1) {
		glUniformMatrix4fv(obj2.getUNormalMat(), 1, GL_FALSE, &(normal[0][0]));
	}
	if (obj2.getUModelMat() != -1) {
		glUniformMatrix4fv(obj2.getUModelMat(), 1, GL_FALSE, &(model2[0][0]));
	}
	if (obj2.getUViewMat() != -1) {
		glUniformMatrix4fv(obj2.getUViewMat(), 1, GL_FALSE, &(view[0][0]));
	}
	if (obj2.getUIntensidad() != -1) {
		glUniform3fv(obj2.getUIntensidad(), 1, &(intensidad[0]));
	}
	if (obj2.getUPosLuz() != -1) {
		glUniform3fv(obj2.getUPosLuz(), 1, &(luzPos[0]));
	}
	glBindVertexArray(vao);

	if (obj2.getUColorTex() != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
		glUniform1i(obj2.getUColorTex(), 0);
	}

	if (obj2.getUEmiTex() != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
		glUniform1i(obj2.getUEmiTex(), 1);
	}
	//Coge la lista de indices activa
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);//Pinta triangulos, que coja el numero de triangulos por 3, que son de tipo uint y empieza a coger elementos en la posicion 0 del array
	
	glutSwapBuffers();//Cambio de buffers una vez pintados

}

void resizeFunc(int width, int height){
	glViewport(0, 0, width, height); //Esquina inferior izquierda
	glutPostRedisplay();
}
void idleFunc(){
	/**/
	model = glm::mat4(1.0f);
	model2 = glm::mat4(1.0f);
	static float angle = 0.0f;
	angle = (angle > 3.141592f*2.0f) ? 0 : angle +0.001f;
	//Primer cubo
	model = translateFather*model;
	model = glm::rotate(model, angle, glm::vec3(1.0f, 1.0f, 1.0f));
	
	//Segundo cubo
	glm::mat4 rotacion2(1.0f);
	glm::mat4 translate2(1.0f);
	glm::mat4 rotacionEnd(1.0f);
	translate2[3].x = 3.0f;
	rotacion2=glm::rotate(rotacion2, angle, glm::vec3(1.0f, 0.0f, 0.0f));
	rotacionEnd = glm::rotate(rotacionEnd, angle, glm::vec3(0.0f, 1.0f, 0.0f));

	model2 =translateFather*rotacionEnd*translate2*rotacion2;
	//*/
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
		//Controles de la luz
	case '+':
		if (intensidad.x < 1) {
			intensidad.x += +0.1;
		}
	break;
	case '-':
		if (intensidad.x >= 0) {
			intensidad.x += -0.1;
		}
		break;
	case 'y':
	case 'Y':
		luzPos.y += 0.2;
	break;
	case 'h':
	case 'H':
		luzPos.y += -0.2;
		break;
	case 'g':
	case 'G':
		luzPos.x += -0.2;
		break;
	case 'j':
	case 'J':
		luzPos.x += 0.2;
		break;
		//Controles del objeto 1
	case '8':
		translateFather[3].z +=-0.2 ;
		break;
	case '5':
		translateFather[3].z += 0.2;
		break;
	case '4':
		translateFather[3].x += -0.2;
		break;
	case '6':
		translateFather[3].x += 0.2;
		break;

}	
}
void mouseFunc(int button, int state, int x, int y){}









