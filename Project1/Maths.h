#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/quaternion.hpp>

using namespace std;

class Maths
{
public:
	static glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, float scale);
	static glm::mat4 createTransformationMatrix(glm::vec2 translation, glm::vec2 scale);
	static glm::mat4 createViewMatrix(Camera camera);
	static glm::mat4 Mat4FromAiMat4(aiMatrix4x4 mat);
	static aiMatrix4x4 aiMat4FromMat4(glm::mat4 mat);
	static glm::vec3 Vec3FromAiVec3(aiVector3D vec);
	static glm::quat QuatFromAiQuat(aiQuaternion quat);
	static glm::mat4 createProjectionMatrix(float FOV, float NEAR_PLANE, float FAR_PLANE);
	static btVector3 glmToBullet(const glm::vec3& v);
	static glm::vec3 bulletToGlm(const btVector3& v);
	static glm::quat bulletToGlm(const btQuaternion& q);
	static btMatrix3x3 glmToBullet(const glm::mat3& m);
	static btTransform glmToBullet(const glm::mat4& m);
	static glm::mat4 bulletToGlm(const btTransform& t);
	static btQuaternion glmToBullet(const glm::quat& q);
	Maths();
	~Maths();

};

