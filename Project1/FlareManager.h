#pragma once

#include "FlareRenderer.h"
#include "Camera.h"

class FlareManager
{
public:
	vec2 CENTER_SCREEN = vec2(0.5f);
	vector<FlareTexture> flareTextures;
	float spacing;

	FlareRenderer * renderer;

	void render(Camera camera, vec3 sunWorldPos);

	void calcFlarePositions(vec2 sunToCenter, vec2 sunCoords);

	vec2 convertToScreenSpace(vec3 worldPos, mat4 viewMat, mat4 projectionMat);

	FlareManager(float spacing, vector<FlareTexture> flareTextures);
	~FlareManager();
};

