#pragma once

#include "Loader.h"

class PostProcessing
{
public:
	RawModel quad;

	void doPostProcessing(int colourTexture);
	void cleanUp();

	void start();
	void end();

	
	PostProcessing();
	~PostProcessing();
};

