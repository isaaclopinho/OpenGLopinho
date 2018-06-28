#include "ShadowFrameBuffer.h"
#include "MasterRenderer.h"
#include "Game.h"

void ShadowFrameBuffer::init()
{
	glGenFramebuffers(1, &depthMapFBO);

	cout << depthMapFBO << endl;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowFrameBuffer::bindFrameBuffer()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowFrameBuffer::unbindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Game::GetInstance()->WIDTH, Game::GetInstance()->HEIGHT);
}

void ShadowFrameBuffer::bindShadowMap()
{
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
}


void ShadowFrameBuffer::renderSceneOnBuffer() {
	bindFrameBuffer();
	renderer->render(MasterRenderer::GetInstance().entities);
	unbindFrameBuffer();
}


ShadowFrameBuffer::ShadowFrameBuffer() : SHADOW_HEIGHT(1024), SHADOW_WIDTH(1024)
{
	renderer = new ShadowMapEntityRenderer(projectionViewMatrix());
	
	init();
}

ShadowFrameBuffer::ShadowFrameBuffer(unsigned int SHADOW_WIDTH, unsigned int SHADOW_HEIGHT): SHADOW_HEIGHT(SHADOW_HEIGHT), SHADOW_WIDTH(SHADOW_WIDTH)
{
	renderer = new ShadowMapEntityRenderer(projectionViewMatrix());
	init();
}


mat4 ShadowFrameBuffer::projectionViewMatrix() {
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	glm::vec3 lightPos = vec3(-2.0, 200.0, -1.0);
	float near_plane = 1.0f, far_plane = 600.5f;
	lightProjection = glm::ortho(-1000.0f, 1000.0f, -100.0f, 600.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	MasterRenderer::GetInstance().lightSpaceMatrix = lightSpaceMatrix;
	return lightSpaceMatrix;
}


ShadowFrameBuffer::~ShadowFrameBuffer()
{
}
