#include "GUIRenderer.h"

#include "Maths.h"

GUIRenderer::GUIRenderer() 
{
	vector<vec2> positions = {
		vec2(-1,1),
		vec2(-1,-1),
		vec2(1,1),
		vec2(1,-1)
	};

	vector<vec2> positions2 = {
		vec2(-1,-1),
		vec2(1,1)
	};

	shader = new GUIShader();

	quad = Loader::loadToVAO(positions);
	line = Loader::loadToVAO(positions2);

}

void GUIRenderer::render(vector<GUITexture> guis)
{
	shader->start();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	

	

	for (int i = 0; i < guis.size(); i++) {

		glBindVertexArray(quad.vaoID);
		glEnableVertexAttribArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, guis[i].textureID);
		shader->loadConstant(guis[i].constant);
		mat4 matrix = Maths::createTransformationMatrix(guis[i].position, guis[i].scale);
		shader->loadTransformationMatrix(matrix);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.vertexCount);
		
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		glBindVertexArray(line.vaoID);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_LINES, 0, line.vertexCount);

		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	shader->stop();
}

GUIRenderer::~GUIRenderer()
{
}
