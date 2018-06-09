#include "ShadowShader.h"


void ShadowShader::loadmvpMatrix(mat4 mvpMatrix)
{
	ShaderProgram::loadMatrix(location_mvpMatrix, mvpMatrix);
}


void ShadowShader::bindAttributes()
{
	bindAttribute(0, "in_position");
	bindAttribute(1, "aTexCoords");
	bindAttribute(2, "aNormal");
	bindAttribute(3, "BoneIDs");
	bindAttribute(4, "pesos");
}


void ShadowShader::LoadBoneMatrices(std::vector<glm::mat4> matrices) {
	for (int i = 0; i < 100; i++) {
		if (i < matrices.size()) {
			loadMatrix(lgBones[i], matrices[i]);
		}
		else {
			loadMatrix(lgBones[i], glm::mat4(1.f));
		}
	}
}

void ShadowShader::getAllUniformLocations()
{
	location_mvpMatrix = ShaderProgram::getUniformLocation("mvpMatrix");
	location_animated = ShaderProgram::getUniformLocation("animated");

	for (int i = 0; i < 100; i++) {
		std::ostringstream local;

		local << "gBones[" << i << "]";
		lgBones[i] = ShaderProgram::getUniformLocation(local.str());
	}
}


ShadowShader::ShadowShader() : ShaderProgram("shaders//shadow.vs", "shaders//shadow.fs")
{
	bindAttributes();
	ShaderProgram::LinkAndValidateProgram();
	getAllUniformLocations();
}


void ShadowShader::LoadBoolAnimated(int value)
{
	loadInt(location_animated, value);
}

ShadowShader::~ShadowShader()
{
}
