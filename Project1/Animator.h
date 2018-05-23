#pragma once

#include <string>
#include <unordered_map>

#include "assimp/anim.h"

#include "Mesh.h"

typedef struct KeyFrame {
	float time;
	aiVector3D positions;
	aiVector3D scaling;
	aiQuaternion rotation;
	aiNodeAnim* nodeAnim;
	int frameIndex;
} KeyFrame;

class Animator {
	enum STATE {
		RUNNING, CHANGING, STOPPED
	};

	aiAnimation* currentAnimation;
	aiAnimation* nextAnimation;
	Mesh* mesh;

	STATE state;
	//Dict of node name and current Keyframe
	std::unordered_map<std::string, KeyFrame> currentFrame;
	//Dict of node name and next Keyframe
	std::unordered_map<std::string, KeyFrame> nextFrame;

	int currentFrameIndex;
	int frameCount;

	float ticksPerSec;
	float timeInTicks;

	float totalTime;

	bool repeat;

	float frameEndTime;

	KeyFrame SetFrame(aiNodeAnim*, int);

	void CalcInterpolatedPosition(aiVector3D&, aiVector3D, aiVector3D, float factor);
	void CalcInterpolatedScaling(aiVector3D&,aiVector3D, aiVector3D, float factor);
	void CalcInterpolatedRotation(aiQuaternion&, aiQuaternion, aiQuaternion, float factor);

public:
	void CalcInterpolatedPosition(glm::vec3 & result, glm::vec3 a, glm::vec3 b, float factor);
	void CalcInterpolatedScaling(glm::vec3 & result, glm::vec3 a, glm::vec3 b, float factor);
	void CalcInterpolatedRotation(glm::quat & result, glm::quat a, glm::quat b, float factor);
	Animator(Mesh* mesh, std::string animationName, bool repeat);
	void BoneTransforms(std::vector<glm::mat4>& transforms);
	void Update(float dt);
	void ChangeAnimation(std::string nextAnimation, float time);
};
