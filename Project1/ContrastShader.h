#pragma once
#include "ShaderProgram.h"
class ContrastShader : public ShaderProgram
{
public:
	
	void bindAttributes();
	void getAllUniformLocations();
	ContrastShader();
	~ContrastShader();
};

