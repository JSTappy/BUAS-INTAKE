#ifndef ITEM_H 
#define ITEM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GameEntity; //forward declaration to prevent dependency loops

class Item
{
public:
	/// @brief constructor
	Item(int itemType, int uses);//item type, amount of items

	/// @brief destructor
	virtual ~Item();

	void ApplyItem(GameEntity* target);

	int GetUses() { return _uses; }



private:
	int _itemType;
	int _uses;
};

#endif // VISUALSLIDER_H 