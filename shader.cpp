#include "shader.h"

#include <gl/glew.h>

#include <iostream>


void shader::initShader(const char *vname, const char *fname) {
	vshader = loadShader(vname, GL_VERTEX_SHADER);  //Creamos los shaders de vertices y fragmentos
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	//Enlazamos
	program = glCreateProgram();//Programa=Conjunto de shaders que trabajan juntos
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	//Asignamos identificadores a los atributos
	glBindAttribLocation(program, 0, "inPos");
	glBindAttribLocation(program, 1, "inColor");
	glBindAttribLocation(program, 2, "inNormal");
	glBindAttribLocation(program, 3, "inTexCoord");
	//Luces
	//glBindAttribLocation(program, 4, "inIntensidad");
	//glBindAttribLocation(program, 5, "inPosLuz");

	glLinkProgram(program);

	//Lo mismo que para los errores en loadShader(), sabiendo el error
	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteProgram(program);
		exit(-1);
	}

	//Variables uniform, pregutnamos por el identificador
	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");
	uModelMat = glGetUniformLocation(program, "model");
	uViewMat = glGetUniformLocation(program, "view");
	//Luces
	uIntensidad = glGetUniformLocation(program, "intensidad");
	uPosLuz = glGetUniformLocation(program, "posLuz");
	//Variables atributos, preguntamos identificador
	inPos = glGetAttribLocation(program, "inPos");
	inColor = glGetAttribLocation(program, "inColor");
	inNormal = glGetAttribLocation(program, "inNormal");
	inTexCoord = glGetAttribLocation(program, "inTexCoord");
	//Variables atributos de las texturas
	uColorTex = glGetUniformLocation(program, "colorTex");
	uEmiTex = glGetUniformLocation(program, "emiTex");

}


//Nombre del fichero y que tipo de shader creamos
GLuint shader::loadShader(const char *fileName, GLenum type) {
	unsigned int fileLen;//Longitud del fichero
	char *source = loadStringFromFile(fileName, fileLen);
	//Creacion y compilacion del shader
	GLuint shader; //Creamos un objeto shader
	shader = glCreateShader(type); //Creamos un shader del tipo indicado
	glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);//Al shader que se acaba de crear, se le pasa una cadena de cadena de caracteres que solo tiene una cadena
	glCompileShader(shader);
	delete[] source; //Borramos las posiciones consecutivas

					 //Comprobamos que se compila
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen); //Comprobamos el tamaño en caracteres del error
		char *logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);//Recogemos el error
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}
	return shader;
}

void shader::activateProgram() {
	glUseProgram(program);
}


void shader::destroy() {
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);
}