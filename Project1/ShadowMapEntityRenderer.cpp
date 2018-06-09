#include "ShadowMapEntityRenderer.h"


void ShadowMapEntityRenderer::render(vector<Entity*> entities)
{
	shader->start();

	bindAttributes();

	for (auto entity : entities) {
		prepareInstance(entity);

		entity->GetMesh()->Render();
	}
	
	unbindAttributes();
	shader->stop();
}


void ShadowMapEntityRenderer::bindAttributes()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
}

void ShadowMapEntityRenderer::unbindAttributes()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glBindVertexArray(0);
}

void ShadowMapEntityRenderer::prepareInstance(Entity * entity)
{
	mat4 modelMatrix = Maths::createTransformationMatrix(entity->position, entity->rotation, entity->scale);
	mat4 mvpMatrix = projectionViewMatrix * modelMatrix;
	shader->loadmvpMatrix(mvpMatrix);
	shader->LoadBoolAnimated(entity->GetMesh()->animated);
	shader->LoadBoneMatrices(entity->transforms);
}

ShadowMapEntityRenderer::ShadowMapEntityRenderer(mat4 projectionViewMatrix) : projectionViewMatrix(projectionViewMatrix)
{
	shader = new ShadowShader();
}

ShadowMapEntityRenderer::~ShadowMapEntityRenderer()
{
}
