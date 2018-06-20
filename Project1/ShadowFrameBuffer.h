#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShadowMapEntityRenderer.h"

class ShadowFrameBuffer
{

public:
	unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
	GLuint depthMapFBO;
	GLuint depthTexture;
	ShadowMapEntityRenderer * renderer;

public:

	void init();
	void bindFrameBuffer();
	void unbindFrameBuffer();
	void bindShadowMap();

	void renderSceneOnBuffer();

	ShadowFrameBuffer();
	ShadowFrameBuffer(unsigned int SHADOW_WIDTH, unsigned int SHADOW_HEIGHT);
	mat4 projectionViewMatrix();
	~ShadowFrameBuffer();
};

