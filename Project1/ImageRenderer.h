#pragma once

#include "Fbo.h"

class ImageRenderer
{
public:

	Fbo * fbo;

	ImageRenderer(int width, int height);
	void renderQuad();
	GLuint getOutputTexture();
	void cleanUp();
	ImageRenderer();
	~ImageRenderer();
};

