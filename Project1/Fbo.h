#pragma once
#include <GL/glew.h>
#include <iostream>

using namespace std;

class Fbo
{
public:

	const int NONE = 0;
	const int DEPTH_TEXTURE = 1;
	const int DEPTH_RENDER_BUFFER = 2;

	bool multisample = false;

	int width, height;
	GLuint frameBuffer;

	GLuint colourTexture;
	GLuint depthTexture;
	GLuint depthBuffer;
	GLuint colourBuffer;

	GLuint depthShadowTexture;

	Fbo(int width, int height, int depthBufferType);
	Fbo(int width, int height);
	void cleanUp();
	void bindFrameBuffer();
	void unbindFrameBuffer();
	void bindToRead();
	void initialiseFrameBuffer(int type);
	void createFrameBuffer();
	void createFrameBufferShadow();
	void createTextureShaderAttachment();
	void createTextureAttachment();

	void createDepthTextureAttachment();
	void resolveToFbo(Fbo & output);
	void resolveToScreen();
	void createMultisampleColourAttachment();
	void createDepthBufferAttachment();
	~Fbo();
};

