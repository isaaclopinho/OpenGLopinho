#include "PostProcessing.h"


void PostProcessing::doPostProcessing(int colourTexture)
{
	start();
	end();
}


void PostProcessing::cleanUp()
{
}


void PostProcessing::start()
{
	glBindVertexArray(quad.vaoID);
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}


void PostProcessing::end()
{
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}


PostProcessing::PostProcessing()
{
	vector<vec2> POSITIONS = { vec2(-1, 1), vec2(-1, -1), vec2(1, 1), vec2(1, -1) };
	quad = Loader::loadToVAO(POSITIONS);
}


PostProcessing::~PostProcessing()
{

}
