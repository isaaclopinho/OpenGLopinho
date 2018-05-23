#include "Animator.h"
#include "Maths.h"

KeyFrame Animator::SetFrame(aiNodeAnim *nodeAnim, int index)
{
	KeyFrame kf;

	kf.nodeAnim = nodeAnim;
	kf.frameIndex = index;
	kf.positions = nodeAnim->mPositionKeys[index].mValue;
	kf.scaling = nodeAnim->mScalingKeys[index].mValue;
	kf.rotation = nodeAnim->mRotationKeys[index].mValue;
	kf.time = nodeAnim->mPositionKeys[index].mTime;

	return kf;
}
/*
void Animator::CalcInterpolatedPosition(aiVector3D &result, aiVector3D a, aiVector3D b, float factor)
{
	result = a + (b - a)*factor;
}

void Animator::CalcInterpolatedScaling(aiVector3D &result, aiVector3D a, aiVector3D b, float factor)
{
	result = a + (b - a)*factor;
}

void Animator::CalcInterpolatedRotation(aiQuaternion &out, aiQuaternion a, aiQuaternion b, float factor)
{
	aiQuaternion helper;
	aiQuaternion::Interpolate(helper, a, b, factor);

	out = helper.Normalize();
}
*/

void Animator::CalcInterpolatedPosition(glm::vec3 &result, glm::vec3 a, glm::vec3 b, float factor) {
	result = a + ((b - a)*factor);
}

void Animator::CalcInterpolatedScaling(glm::vec3 &result, glm::vec3 a, glm::vec3 b, float factor) {
	result = a + (b - a)*factor;
}

void Animator::CalcInterpolatedRotation(glm::quat &result, glm::quat a, glm::quat b, float factor) {
	result = glm::lerp(a, b, factor);
}

Animator::Animator(Mesh * mesh, std::string animationName, bool repeat): mesh(mesh), repeat(repeat)
{
	currentAnimation = mesh->animationMap[animationName];
	nextAnimation = NULL;

	ticksPerSec = currentAnimation->mTicksPerSecond;
	timeInTicks = 0;

	totalTime = currentAnimation->mDuration;

	currentFrameIndex = 0;

	for (int i = 0; i < currentAnimation->mNumChannels; i++) {
		aiNodeAnim* currentNodeAnim = currentAnimation->mChannels[i];

		currentFrame[currentNodeAnim->mNodeName.C_Str()] = SetFrame(currentNodeAnim, 0);

		nextFrame[currentNodeAnim->mNodeName.C_Str()] = SetFrame(currentNodeAnim, 1);

		frameCount = currentNodeAnim->mNumPositionKeys;
	}

	frameEndTime = nextFrame.begin()->second.time;


	state = RUNNING;
}

void Animator::BoneTransforms(std::vector<glm::mat4>& transforms)
{
	glm::mat4 identity;

	std::queue<aiNode*>nodes;
	std::queue<glm::mat4>matrices;

	nodes.push(mesh->mpScene->mRootNode);
	matrices.push(identity);

	while (!nodes.empty()) {
		aiNode* pNode = nodes.front();
		nodes.pop();

		glm::mat4 parentTransform = matrices.front();
		matrices.pop();

		std::string nodeName(pNode->mName.data);

		glm::mat4  NodeTransformation = (Maths::Mat4FromAiMat4(pNode->mTransformation));

		if (currentFrame.find(nodeName) != currentFrame.end()) {
			float factor = timeInTicks - currentFrame[nodeName].time;
			//factor = factor/(nextFrame[nodeName].time - currentFrame[nodeName].time);

			// Interpolate scaling and generate scaling transformation matrix
			//aiVector3D scaling;
			//CalcInterpolatedScaling(scaling, currentFrame[nodeName].scaling, nextFrame[nodeName].scaling, factor);
			glm::vec3 scaling;
			CalcInterpolatedScaling(scaling, Maths::Vec3FromAiVec3(currentFrame[nodeName].scaling), Maths::Vec3FromAiVec3(nextFrame[nodeName].scaling), factor);
			glm::mat4 scalingM;

			scalingM = glm::scale(scalingM, scaling);

			// Interpolate rotation and generate rotation transformation matrix
			glm::quat rotationQ;
			CalcInterpolatedRotation(rotationQ, Maths::QuatFromAiQuat(currentFrame[nodeName].rotation), Maths::QuatFromAiQuat(nextFrame[nodeName].rotation), factor);
			glm::mat4 rotationM = glm::toMat4(rotationQ);

			// Interpolate translation and generate translation transformation matrix
			glm::vec3 translation;
			CalcInterpolatedPosition(translation, Maths::Vec3FromAiVec3(currentFrame[nodeName].positions), Maths::Vec3FromAiVec3(nextFrame[nodeName].positions), factor);
			glm::mat4 translationM;

			translationM = glm::translate(translationM, translation);

			// Combine the above transformations
			NodeTransformation = translationM * rotationM * scalingM;
		}

		glm::mat4 GlobalTransformation = parentTransform * NodeTransformation;

		if (mesh->boneMapping.find(nodeName) != mesh->boneMapping.end()) {
			unsigned int boneIndex = mesh->boneMapping[nodeName];
			auto aux = mesh->mGlobalInverseTransform * GlobalTransformation * mesh->mBoneInfo[boneIndex].boneOffset;
			mesh->mBoneInfo[boneIndex].finalTransform = mesh->mGlobalInverseTransform * GlobalTransformation * mesh->mBoneInfo[boneIndex].boneOffset;
		}

		for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
			nodes.push(pNode->mChildren[i]);
			matrices.push(GlobalTransformation);
			//ReadNodeHierarchy(animationTime, pNode->mChildren[i], GlobalTransformation);
		}
	}

	//ReadNodeHierarchy(animationTime, mpScene->mRootNode, identity);

	transforms.resize(mesh->NumBones());

	for (unsigned int i = 0; i <mesh->NumBones(); i++) {
		transforms[i] = mesh->mBoneInfo[i].finalTransform;
	}
}

void Animator::Update(float dt)
{
	if(state != STOPPED)
		timeInTicks += dt * ticksPerSec;

	if (timeInTicks >= frameEndTime) {
		switch (state) {
		case RUNNING:
		{
			++currentFrameIndex;

			for (int i = 0; i < currentAnimation->mChannels[0]->mNumPositionKeys; i++) {
				if (timeInTicks <= currentAnimation->mChannels[0]->mPositionKeys[i].mTime) {
					currentFrameIndex = (!i)?i-1:i;
					break;
				}
			}


			if (currentFrameIndex == frameCount - 1) {
				if (repeat) {
					currentFrameIndex = 0;
					timeInTicks = currentAnimation->mChannels[0]->mPositionKeys[0].mTime;
				}
				else {
					state = STOPPED;
				}
			}

			currentFrame = nextFrame;
			timeInTicks = currentFrame.begin()->second.time;
			for (auto itr = nextFrame.begin(); itr != nextFrame.end(); ++itr) {
				aiNodeAnim* nodeAnim = itr->second.nodeAnim;

				//currentFrame[itr->first] = SetFrame(nodeAnim, currentFrameIndex);
				itr->second = SetFrame(nodeAnim, currentFrameIndex + 1);
				//timeInTicks = currentFrame[itr->first].time;
				frameEndTime = itr->second.time;
			}
			
			break;
		}
		case CHANGING:
		{
			currentFrameIndex = 0;
			currentAnimation = nextAnimation;
			nextAnimation = NULL;

			state = RUNNING;

			currentFrame = nextFrame;
			for (auto itr = nextFrame.begin(); itr != nextFrame.end(); itr++) {
				aiNodeAnim* nodeAnim = itr->second.nodeAnim;

				itr->second = SetFrame(nodeAnim, currentFrameIndex + 1);

				frameEndTime = itr->second.time;
			}
		}

		} //EndSwitch
	}
}
