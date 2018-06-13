#pragma once
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include "Maths.h"
#include <vector>

using namespace glm;

class FlareShader :
	public ShaderProgram
{
public:
	GLuint location_brightness;
	GLuint location_transform;
	GLuint location_flareTexture;
	
	void connectMaps();
	void loadBrightness(float bright);
	void loadTransform(vec4 transform);
	void bindAttributes();
	void getAllUniformLocations();
	FlareShader();
	~FlareShader();
};

