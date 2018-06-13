#include "FlareShader.h"


void FlareShader::connectMaps()
{
	ShaderProgram::loadInt(location_flareTexture, 0);
}

void FlareShader::loadBrightness(float b)
{
	ShaderProgram::loadFloat(location_brightness, b);
}


void FlareShader::loadTransform(vec4 v)
{
	ShaderProgram::loadVector4D(location_brightness, v);
}

void FlareShader::bindAttributes()
{
	bindAttribute(0, "in_position");
}

void FlareShader::getAllUniformLocations()
{
	location_brightness = ShaderProgram::getUniformLocation("brightness");
	location_transform = ShaderProgram::getUniformLocation("transform");
	location_flareTexture = ShaderProgram::getUniformLocation("flareTexture");
}

FlareShader::FlareShader() : ShaderProgram("shaders//flare.vs", "shaders//flare.fs")
{
	bindAttributes();
	ShaderProgram::LinkAndValidateProgram();
	getAllUniformLocations();
}


FlareShader::~FlareShader()
{
}
