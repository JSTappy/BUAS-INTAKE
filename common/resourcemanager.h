#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include "sprite.h"

class ResourceManager
{
public:
	ResourceManager();

	virtual ~ResourceManager();

	Sprite* GetTarga(const std::string& fileName);


private:
	std::map<std::string, Sprite*> _targas;

};

#endif // !RESOURCEMANAGER_H