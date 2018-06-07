#pragma once

#include "ImageRenderer.h"
#include "ContrastShader.h"

class ContrastChanger
{
public:

	ImageRenderer * renderer;
	ContrastShader * shader;

	void render(GLuint texture);
	void CleanUp();
	ContrastChanger();
	~ContrastChanger();
};

