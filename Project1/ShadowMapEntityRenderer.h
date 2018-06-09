#pragma once

#include <unordered_map>
#include "ShadowShader.h"
#include "Entity.h"


class ShadowMapEntityRenderer
{
public:

	mat4 projectionViewMatrix;
	ShadowShader * shader;

	void render( vector<Entity*> entities);
	void bindAttributes();
	void unbindAttributes();
	void prepareInstance(Entity * entity);
	ShadowMapEntityRenderer(mat4 projectionViewMatrix);
	~ShadowMapEntityRenderer();
};

