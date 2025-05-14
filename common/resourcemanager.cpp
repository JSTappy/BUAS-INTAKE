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
	//If the targa has already been added to the list, return it
	if (_targas[fileName] != NULL)
	{
		return _targas[fileName];
	}

	//Make a new sprite, load it and put it in the targas list
	Sprite* s = new Sprite(fileName);
	s->loadTGA(fileName);
	_targas[fileName] = s;
	return s;
}