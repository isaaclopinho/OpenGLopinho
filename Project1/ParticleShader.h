#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
#include <glm/glm.hpp>

using namespace glm;

class ParticleShader : public ShaderProgram
{
public:

	GLuint location_projectionMatrix;
	GLuint location_texNumberOfRows;
	
	ParticleShader();
	~ParticleShader();
	void loadProjectionMatrix(mat4 matrix);
	void bindAttributes();
	void loadNumberOfRows(float rows);
	void getAllUniformLocations();
};

