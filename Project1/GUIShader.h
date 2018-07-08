#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
#include "Maths.h"

using namespace glm;

class GUIShader :
	public ShaderProgram
{
private:
	GLint location_transformationMatrix;
	GLint location_constant;
public:
	void bindAttributes();
	void getAllUniformLocations();	
	void loadTransformationMatrix(mat4 matrix);
	void loadConstant(float constant);
	GUIShader();
	~GUIShader();
};

