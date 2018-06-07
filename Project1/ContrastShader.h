#pragma once
#include "ShaderProgram.h"
class ContrastShader : public ShaderProgram
{
public:
	
	GLuint location_offset;

	void loadOffset(float v);
	void bindAttributes();
	void getAllUniformLocations();
	ContrastShader();
	~ContrastShader();
};

