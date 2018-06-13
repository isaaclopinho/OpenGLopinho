#include "FlareRenderer.h"
#include "Game.h"


void FlareRenderer::render(vector<FlareTexture> flares, float brightness)
{

	prepare(brightness);
	for (int i = 0; i < flares.size(); i++) {
		renderFlare(flares[i]);
	}
	endRendering();
}

void FlareRenderer::prepare(float brightness)
{
	shader->start();
	shader->loadBrightness(brightness);

	glBindVertexArray(quad.vaoID);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glDepthMask(false);
	

}

void FlareRenderer::renderFlare(FlareTexture flare)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, flare.texture);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	vec2 scale = vec2(flare.scale, flare.scale * Game::GetInstance()->GetAspectRatio());
	vec2 centerPos = flare.screenPos;
	vec4 transform = vec4(centerPos.x, centerPos.y, scale.x, scale.y);
	shader->loadTransform(transform);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void FlareRenderer::endRendering()
{
	glDepthMask(true);
	glDisable(GL_BLEND);
	glBindVertexArray(quad.vaoID);
	glDisableVertexAttribArray(0);
	shader->stop();
}

FlareRenderer::FlareRenderer()
{
	vector<vec2> POSITIONS = {vec2 (-0.5f, -0.5f), vec2(-0.5f, 0.5f), vec2(0.5f, -0.5f), vec2(0.5f, 0.5f) };

	quad = Loader::loadToVAO(POSITIONS);
	shader = new FlareShader();
	
}


FlareRenderer::~FlareRenderer()
{
}
