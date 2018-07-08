#include "GameObjectTest.h"


GameObjectTest::GameObjectTest(Entity entity) : entity(entity), GameObject()
{
}

GameObjectTest::~GameObjectTest()
{
}

void GameObjectTest::Update(float dt)
{
	//entity.Rotate(vec3(0, 1, 0));
	if(animated)
		entity.Update(dt);
}

void GameObjectTest::Render()
{
	MasterRenderer::GetInstance().processEntity(&entity);
}
