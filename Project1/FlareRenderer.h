#pragma once

#include "RawModel.h"
#include "Loader.h"
#include "GUITexture.h"
#include <glm/glm.hpp>
#include "FlareShader.h"
#include "FlareTexture.h"

using namespace glm;
using namespace std;

class FlareRenderer
{
public:

	FlareShader * shader;
	RawModel quad;

	void render(vector<FlareTexture> flares, float brightness);
	void prepare(float brightness);
	void renderFlare(FlareTexture flare);
	void endRendering();

	FlareRenderer();
	~FlareRenderer();
};

