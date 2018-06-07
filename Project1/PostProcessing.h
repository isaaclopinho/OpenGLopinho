#pragma once

#include "Loader.h"
#include "RawModel.h"
#include "ConstrastChanger.h"

class PostProcessing
{
public:
	RawModel quad;
	ContrastChanger cc;

	void doPostProcessing(GLuint colourTexture);
	void cleanUp();
	void init();
	void start();
	void end();

	~PostProcessing();
};

