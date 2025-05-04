#include "myentity.h"

MyEntity::MyEntity()
{

}

MyEntity::~MyEntity()
{

}

void MyEntity::Update(float deltaTime)
{
	if (GetInput()->GetKey(KEY_O))
	{
		this->position += glm::vec3(0.0f, 0.5f, 0.0f);
	}
}

void MyEntity::ChangeSprite(const std::string& imagepath)
{
	this->sprite = new Sprite(imagepath);
}