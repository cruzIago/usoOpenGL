#ifndef SHADER_H_
#define SHADER_H_

#include "auxiliar.h"

#include <gl/glew.h>

#include <iostream>


class shader {
private:

	unsigned int vshader; //identificador del shader de vertices
	unsigned int fshader; //identificador del shader de fragmentos
	unsigned int program; //identificador del programa (conjunto de shaders)

	//Variables uniform
	int uModelViewMat;   //acceso a la variable modelView de opengl
	int uModelViewProjMat; //acceso a la variable modelViewProj de opengl
	int uModelMat;			//Acceso a la variable model de opengl
	int uNormalMat;		  //acceso a la variable normalMat de opengl
	int uViewMat;			//Acceso a la variable view de opengl
	int uColorTex;			//Acceso al identificadorde la textura
	int uEmiTex;

	//Luces uniform
	int uIntensidad;
	int uPosLuz;

	//Atributos
	//Forma de identificarlo para el acceso
	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;

public:
	shader() {}
	
	inline int getInPos() {return inPos;}
	inline int getInColor() { return inColor;}
	inline int getInNormal() { return inNormal;}
	inline int getInTexCoord() { return inTexCoord;}

	inline int getUModelMat() { return uModelMat;}
	inline int getUModelViewMat() { return uModelViewMat; }
	inline int getUModelViewProjMat() { return uModelViewProjMat; }
	inline int getUViewMat() { return uViewMat; }
	inline int getUNormalMat() { return uNormalMat; }
	inline int getUColorTex() { return uColorTex; }
	inline int getUEmiTex() { return uEmiTex; }

	inline int getUPosLuz() { return uPosLuz; }
	inline int getUIntensidad() { return uIntensidad; }

	void initShader(const char *vname, const char*fname);
	GLuint loadShader(const char *fileName, GLenum type);
	void activateProgram();
	void destroy();
	//TODO 
	//Shader renderfunc
};

#endif
