#include "ParticleShader.h"
#include "Debug.h"
ParticleShader::ParticleShader() : ShaderProgram("shaders//particle.vs", "shaders//particle.fs")
{
	bindAttributes();
	ShaderProgram::LinkAndValidateProgram();
	getAllUniformLocations();
}

ParticleShader::~ParticleShader()
{
}

void ParticleShader::loadProjectionMatrix(mat4 matrix)
{
	ShaderProgram::loadMatrix(location_projectionMatrix, matrix);
}

void ParticleShader::bindAttributes() {
	//"ParticleShaderBindAttributes");
	bindAttribute(0, "position");
	bindAttribute(1, "modelViewMatrix");
	bindAttribute(5, "texOffsets");
	bindAttribute(6, "blendFactor");
}

void ParticleShader::loadNumberOfRows(float rows)
{
	ShaderProgram::loadFloat(location_texNumberOfRows, rows);
}


void ParticleShader::getAllUniformLocations() {
	location_projectionMatrix = ShaderProgram::getUniformLocation("projectionMatrix");
	location_texNumberOfRows = ShaderProgram::getUniformLocation("numberOfRows");

}
