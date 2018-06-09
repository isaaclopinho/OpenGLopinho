#pragma once
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include "Maths.h"
#include <vector>

using namespace glm;

class ShadowShader :
	public ShaderProgram
{
public:
	GLuint location_mvpMatrix;
	GLuint location_animated;
	int lgBones[100];

	void loadmvpMatrix(mat4 mvpMatrix);
	void LoadBoolAnimated(int value);
	void LoadBoneMatrices(vector<glm::mat4> matrices);
	void bindAttributes();
	void getAllUniformLocations();
	ShadowShader();
	~ShadowShader();
};

