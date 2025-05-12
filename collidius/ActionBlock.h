#ifndef ACTIONBLOCK_H 
#define ACTIONBLOCK_H

#include "entity.h"
#include "CommandWindow.h"

class ActionBlock : public Entity
{
public:
	/// @brief constructor
	ActionBlock();

	/// @brief destructor
	virtual ~ActionBlock();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update
	void NextBlock();
	void PreviousBlock();
	void SelectBlock();
	void OpenCommandWindow();

	void CleanUp();

	glm::vec2 ConfirmAction();

private:
	CommandWindow* _commandWindow;

	std::string _attackSprite = ("assets/sprites/attacksprite.tga");
	std::string _defendSprite = ("assets/sprites/defendsprite.tga");
	std::string _itemSprite = ("assets/sprites/itemsprite.tga");

	std::vector<std::string> _actionBlocks;

	int _spriteCounter;
	int _commandWindowIsActive;
};

#endif // ACTIONBLOCK_H 