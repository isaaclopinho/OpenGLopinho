#include "ConstrastChanger.h"



void ContrastChanger::render(GLuint texture)
{
	shader->start();
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
