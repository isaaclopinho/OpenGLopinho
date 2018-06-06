#include "ImageRenderer.h"



ImageRenderer::ImageRenderer(int width, int height)
{
	fbo = new Fbo(width, height, 0);
}

void ImageRenderer::renderQuad()
{
	if (fbo != nullptr) {
		fbo->bindFrameBuffer();
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	if (fbo != nullptr) {
		fbo->unbindFrameBuffer();
	}
}

GLuint ImageRenderer::getOutputTexture()
{
	return fbo->colourTexture;
}

void ImageRenderer::cleanUp()
{
	if (fbo != nullptr) {
		fbo->cleanUp();
	}
}

ImageRenderer::ImageRenderer()
{
}


ImageRenderer::~ImageRenderer()
{
}
