#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;


class FlareTexture
{
public:

	GLuint texture;
	float scale;

	vec2 screenPos;

	FlareTexture(GLuint texture, float scale);
	~FlareTexture();
};

