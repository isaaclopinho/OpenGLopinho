#pragma once
#include "ParticleShader.h"
#include "RawModel.h"
#include "Particle.h"
#include <vector>
#include "ParticleTexture.h"
#include <unordered_map>

class ParticleRenderer
{
public:
	const int MAX_INSTANCES = 10000;
	const int INSTANCE_DATA_LENGTH = 21;
	int pointer = 0;
	RawModel quad;
	ParticleShader * shader;

	int vbo;

	ParticleRenderer(ParticleShader * shader, mat4 proj);
	~ParticleRenderer();
	void updateModelViewMatrix(vec3 pos, float rotation, float scale, mat4 view, vector<float> &f);
	void storeMatrixData(mat4 matrix, vector<float> &f);
	void storeTexCoords(Particle particle, vector<float> &f);
	void render(unordered_map<ParticleTexture, vector<Particle>> particles, Camera camera);
	void cleanUp();
	void prepare();
	void finishRendering();
};

