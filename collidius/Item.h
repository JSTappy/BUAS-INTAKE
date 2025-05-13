#ifndef ITEM_H 
#define ITEM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Audio/Sound.hpp>

class GameEntity; //forward declaration to prevent dependency loops

class Item
{
public:
	/// @brief constructor
	Item(char itemType, char uses);//item type, amount of items

	/// @brief destructor
	virtual ~Item();

	void ApplyItem(GameEntity* target);

	char GetUses() { return _uses; }



private:
	char _itemType;
	char _uses;

	Audio::Sound* _itemSound = new Audio::Sound("assets/audio/useitem.ogg", Audio::Sound::Type::Sound);
};

#endif // VISUALSLIDER_H 