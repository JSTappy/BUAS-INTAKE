#include <iostream>
#include "scene.h"
#include "config.h"

Scene::Scene()
{
	_camera = new Camera();
}

Scene::~Scene()
{
	// Delete all the entities
	for (size_t i = 0; i < GetChildren().size(); i++)
	{
		delete GetChildren()[i];
		GetChildren()[i] = nullptr;
	}
	GetChildren().clear();

	// Delete camera
	delete _camera;
}

void Scene::AddSprites()
{
	for (size_t i = 0; i < GetChildren().size(); i++)
	{
		_sprites.push_back(GetChildren()[i]->sprite);
	}
}

void Scene::AddSprite(Sprite* sprite) {
	_sprites.push_back(sprite);
}
