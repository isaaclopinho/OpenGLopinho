#include "ContrastShader.h"



void ContrastShader::loadOffset(float v)
{
	ShaderProgram::loadFloat(location_offset, v);
}

void ContrastShader::bindAttributes()
{
	bindAttribute(0, "position");
}

void ContrastShader::getAllUniformLocations()
{
	location_offset = ShaderProgram::getUniformLocation("offset");
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
