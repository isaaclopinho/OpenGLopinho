#include "ConstrastChanger.h"

#include <SDL.h>

void ContrastChanger::render(GLuint texture)
{
	shader->start();
	shader->loadOffset(SDL_GetTicks() / 1000.0 * 2 * 3.14159 * .75);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	renderer->renderQuad();
	shader->stop();
}

void ContrastChanger::CleanUp()
{	
	renderer->cleanUp();
}

ContrastChanger::ContrastChanger()
{
	shader = new ContrastShader();
	renderer = new ImageRenderer();
}


ContrastChanger::~ContrastChanger()
{
}
