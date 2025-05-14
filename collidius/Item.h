#ifndef ITEM_H 
#define ITEM_H

#include <Audio/Sound.hpp>

class GameEntity; //forward declaration to prevent dependency loops

class Item
{
public:
	/// @brief constructor
	Item(char itemType, char uses);//item type, amount of items

	/// @brief destructor
	virtual ~Item();

	///@brief Apply the item to the target
	///@param GameEntity* target, the target the item is being applied 
	void ApplyItem(GameEntity* target);

	///@brief Get the amount of uses this item has left
	///@return _uses
	char GetUses() { return _uses; }
private:
	///@brief the item type
	char _itemType;

	///@brief Gthe amount of uses this item has left
	char _uses;

	///@brief the sound that plays when using an item 
	Audio::Sound* _itemSound = new Audio::Sound("assets/audio/useitem.ogg", Audio::Sound::Type::Sound);
};

#endif // VISUALSLIDER_H 