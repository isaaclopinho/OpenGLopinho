#include "Fbo.h"
#include "Game.h"
void Fbo::bindFrameBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

void Fbo::unbindFrameBuffer()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT);
}

void Fbo::bindToRead()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void Fbo::initialiseFrameBuffer(int type)
{
	createFrameBuffer();
	createTextureAttachment();

	if (type == DEPTH_RENDER_BUFFER)
		createDepthBufferAttachment();
	else if (type == DEPTH_TEXTURE)
		createDepthTextureAttachment();

	unbindFrameBuffer();
}

void Fbo::createFrameBuffer()
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void Fbo::createTextureAttachment()
{
	glGenTextures(1, &colourTexture);
	glBindTexture(GL_TEXTURE_2D, colourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0);

}

void Fbo::createDepthTextureAttachment()
{

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

void Fbo::createDepthBufferAttachment()
{
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

Fbo::~Fbo()
{
}

Fbo::Fbo(int width, int height, int depthBufferType) : width(width), height(height)
{
	initialiseFrameBuffer(depthBufferType);
	std::cout << width << " " << height << "\n";
	std::cout <<" fb: " <<  frameBuffer << " colortEXUTRE: " << colourTexture << "depthTex: " << depthTexture << " DepthBuff: " << depthBuffer << " \n";
}

void Fbo::cleanUp()
{
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteTextures(1, &colourTexture);
	glDeleteTextures(1, &depthTexture);
	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteRenderbuffers(1, &colourBuffer);
}
