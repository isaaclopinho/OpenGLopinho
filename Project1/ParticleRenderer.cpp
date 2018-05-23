#include "ParticleRenderer.h"
#include "Debug.h"
#include "Loader.h"
#include <glm\gtc\matrix_transform.hpp>

ParticleRenderer::ParticleRenderer(ParticleShader * shader, mat4 proj): shader(shader)
{
	vector<vec2> VERTICES = { vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5f), vec2(0.5f, 0.5f), vec2(0.5f, -0.5f) };

	quad = Loader::loadToVAO(VERTICES);

	vbo = Loader::CreateEmptyVBO(INSTANCE_DATA_LENGTH * MAX_INSTANCES);

	Loader::addInstancedAttribute(quad.vaoID, vbo, 1, 4, INSTANCE_DATA_LENGTH, 0);
	Loader::addInstancedAttribute(quad.vaoID, vbo, 2, 4, INSTANCE_DATA_LENGTH, 4);
	Loader::addInstancedAttribute(quad.vaoID, vbo, 3, 4, INSTANCE_DATA_LENGTH, 8);
	Loader::addInstancedAttribute(quad.vaoID, vbo, 4, 4, INSTANCE_DATA_LENGTH, 12);
	Loader::addInstancedAttribute(quad.vaoID, vbo, 5, 4, INSTANCE_DATA_LENGTH, 16);
	Loader::addInstancedAttribute(quad.vaoID, vbo, 6, 1, INSTANCE_DATA_LENGTH, 20);

	shader->start();
	shader->loadProjectionMatrix(proj);
	shader->stop();



}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::updateModelViewMatrix(vec3 pos, float rotation, float scale, mat4 view, vector<float> &f) {
	glm::mat4 matrix(1);
	matrix = glm::translate(matrix, pos);

	matrix[0][0] = view[0][0];
	matrix[1][0] = view[0][1];
	matrix[2][0] = view[0][2];

	matrix[0][1] = view[1][0];
	matrix[1][1] = view[1][1];
	matrix[2][1] = view[1][2];

	matrix[0][2] = view[2][0];
	matrix[1][2] = view[2][1];
	matrix[2][2] = view[2][2];

	matrix = rotate(matrix, glm::radians(rotation), glm::vec3(0, 0, 1));
	matrix = glm::scale(matrix, vec3(scale, scale, scale));

	mat4 mvm = view * matrix;
	storeMatrixData(mvm, f);

}


void ParticleRenderer::storeMatrixData(mat4 matrix, vector<float> & f) {
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			f[pointer++] = matrix[i][j];
			
		}
	}
}

void ParticleRenderer::storeTexCoords(Particle particle, vector<float>& f) {
	f[pointer++] = particle.texOffset1.x;
	f[pointer++] = particle.texOffset1.y;
	f[pointer++] = particle.texOffset2.x;
	f[pointer++] = particle.texOffset2.y;
	f[pointer++] = particle.blend;
}

void ParticleRenderer::render(unordered_map<ParticleTexture, vector<Particle>> particles, Camera camera)
{
	mat4 viewMatrix = Maths::createViewMatrix(camera);
	prepare();

	for (auto&& it : particles) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, it.first.textureID);
		shader->loadNumberOfRows(it.first.numberOfRows);

		pointer = 0;

		vector<float> vboData(it.second.size() * INSTANCE_DATA_LENGTH);
		//Debug(vboData.size());

		for (int i = 0; i < it.second.size(); i++) {
			updateModelViewMatrix(it.second[i].position, it.second[i].rotation, it.second[i].scale, viewMatrix, vboData);
			//shader->loadTextureCoordInfo(it.second[i].texOffset1, it.second[i].texOffset2, it.first.numberOfRows, it.second[i].blend);
			//glDrawArrays(GL_TRIANGLE_STRIP, 0, quad.vertexCount);
			storeTexCoords(it.second[i], vboData);

		}

		Loader::UpdateVBO(vbo, vboData);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, quad.vertexCount, it.second.size());
		//vboData.clear();
	}
	

	finishRendering();
}

void ParticleRenderer::cleanUp()
{
}

void ParticleRenderer::prepare()
{
	shader->start();
	glBindVertexArray(quad.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);


}

void ParticleRenderer::finishRendering()
{
	glDepthMask(true);
	glDisable(GL_BLEND);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glBindVertexArray(0);
	shader->stop();
	
}
