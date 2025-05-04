#include "resourcemanager.h"
#include "config.h"
#include <iostream>

ResourceManager::ResourceManager()
{
	std::cout << "resourcemanager created" << std::endl;
}

ResourceManager::~ResourceManager()
{
	std::cout << "resourcemanager closed" << std::endl;
}

Sprite* ResourceManager::GetTarga(const std::string& fileName) //Getting Targas
{
	if (_targas[fileName] != NULL)
	{
		return _targas[fileName];
	}
	Sprite* s = new Sprite(fileName);
	s->loadTGA(fileName);
	_targas[fileName] = s;
	return s;
}