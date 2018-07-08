#include "GUIShader.h"
#include "Debug.h"


GUIShader::GUIShader() :ShaderProgram("shaders//gui.vs", "shaders//gui.fs")
{
	bindAttributes();
	ShaderProgram::LinkAndValidateProgram();
	getAllUniformLocations();
}

GUIShader::~GUIShader()
{
}

void GUIShader::bindAttributes() {
	bindAttribute(0, "position");
}

void GUIShader::getAllUniformLocations() {
	location_transformationMatrix = ShaderProgram::getUniformLocation("transformationMatrix");
	location_constant = ShaderProgram::getUniformLocation("constant");
}

void GUIShader::loadTransformationMatrix(mat4 matrix)
{
	ShaderProgram::loadMatrix(location_transformationMatrix, matrix);
}

void GUIShader::loadConstant(float constant)
{
	ShaderProgram::loadFloat(location_constant, constant);
}


