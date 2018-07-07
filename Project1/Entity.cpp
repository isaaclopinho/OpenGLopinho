#include "Entity.h"
#include "Debug.h"

void Entity::Translate(glm::vec3 pos)
{
	position += pos;
}

void Entity::Rotate(glm::vec3 rot)
{
	rotation += rot;
}

//float Entity::getTextureXOffset()
//{
//	int column = textureIndex % model.texture.rows;
//	return (float)column/(float)model.texture.rows;
//}
//
//float Entity::getTextureYOffset()
//{
//	int rows = textureIndex / model.texture.rows;
//	return (float)rows / (float)model.texture.rows;
//}

Entity::Entity(Mesh * m, glm::vec3 p, glm::vec3 r, vec3 s, std::string startAnimation, bool loopAnimation) : mesh(m), position(p), rotation(r), scale(s), time(0), currentAnimation(startAnimation), loopAnimation(loopAnimation)/*, anim(m, "Run", true)*/
{
}

Mesh* Entity::GetMesh()
{
	return mesh;
}

void Entity::Update(float dt)
{

	time += dt;
    if(mesh->animated)
        mesh->GetTransforms(currentAnimation, transforms, time, loopAnimation);
	//anim.Update(dt);
	//anim.BoneTransforms(transforms);

}

std::vector<glm::mat4> Entity::GetTransforms()
{
	return transforms;
}
void Entity::ChangeAnimation(std::string newAnimation, bool loops)
{
	loopAnimation = loops;
	currentAnimation = newAnimation;
	time = 0;
}
/*
Entity::Entity(glm::vec3 position, glm::vec3 rotation, float scale, int textureIndex) : position(position), rotation(rotation), scale(scale), textureIndex(textureIndex)
{
	//"GameObject Position", position.x, position.y, position.z);
	//"GameObject Rotation", rotation.x, rotation.y, rotation.z);
	//"GameObject Scale", scale);

	//"GameObject Model", model.rawModel.vaoID, model.rawModel.vertexCount, model.texture.textureID);
}*/

Entity::~Entity()
{
}
