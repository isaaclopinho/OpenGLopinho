#include "ContrastShader.h"



void ContrastShader::bindAttributes()
{
	bindAttribute(0, "position");
}

void ContrastShader::getAllUniformLocations()
{
}


ContrastShader::ContrastShader() : ShaderProgram("shaders//contrast.vs", "shaders//contrast.fs")
{
	bindAttributes();
	ShaderProgram::LinkAndValidateProgram();
	getAllUniformLocations();
}


ContrastShader::~ContrastShader()
{
}
