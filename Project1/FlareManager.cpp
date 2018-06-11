#include "FlareManager.h"
#include "Maths.h"


void FlareManager::render(Camera camera, vec3 sunWorldPos)
{
	
	vec2 sunCoords = convertToScreenSpace(sunWorldPos, camera.viewMatrix, camera.projectionMatrix);


	if (sunCoords == vec2(FLT_MAX, FLT_MAX))
		return;

	vec2 sunToCenter = CENTER_SCREEN - sunCoords;
	float brightness = 1 - (length(sunToCenter) / 0.7f);

	if (brightness > 0) {
		calcFlarePositions(sunToCenter, sunCoords);
		renderer->render(flareTextures, brightness);
	}

}

void FlareManager::calcFlarePositions(vec2 sunToCenter, vec2 sunCoords)
{
	for (int i = 0; i < flareTextures.size(); i++) {
		vec2 direction = sunToCenter * (i * spacing);
		vec2 flarePos = sunCoords + direction;
		flareTextures[i].screenPos = flarePos;
	}

}

vec2 FlareManager::convertToScreenSpace(vec3 worldPos, mat4 viewMat, mat4 projectionMat)
{
	vec4 coords = viewMat * vec4(worldPos, 1);
	coords = projectionMat * coords;

	if (coords.w <= 0)
		return vec2(FLT_MAX, FLT_MAX);

	float x = (coords.x / coords.w + 1.0) / 2.0f;
	float y = 1 - ((coords.y / coords.w + 1.0) / 2.0f);
	return vec2(x,y);
}

FlareManager::FlareManager(float spacing, vector<FlareTexture> flareTextures): spacing(spacing), flareTextures(flareTextures)
{
	renderer = new FlareRenderer();
}

FlareManager::~FlareManager()
{
}
