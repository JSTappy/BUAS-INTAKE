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

	///@brief update method
	///@param deltaTime
	///@return void
	virtual void Update(float deltaTime) {}; //update

	///@brief Go to the next Action Block
	void NextBlock();

	///@brief Go to the previous Action Block
	void PreviousBlock();

	///@brief Select the blocks
	///@param unsigned char action, the type of action being displayed
	void SelectBlock();

	///@brief Displays the commands of an action
	void OpenCommandWindow();

	///@brief Cleans up / Deletes things that should be deleted
	void CleanUp();

	///@brief Get the Action index and the Action type
	///@return glm::vec2(_spriteCounter, _commandWindow->GetSelectedAction()),;
	glm::vec2 GetConfirmedAction() {return glm::vec2(_spriteCounter, _commandWindow->GetSelectedAction());}

private:
	///@brief The command window
	CommandWindow* _commandWindow;

	///@brief The sprite of the attacking action block
	std::string _attackSprite = ("assets/sprites/attacksprite.tga");

	///@brief The sprite of the defending action block
	std::string _defendSprite = ("assets/sprites/defendsprite.tga");

	///@brief The sprite of the item action block
	std::string _itemSprite = ("assets/sprites/itemsprite.tga");

	///@brief List of action block sprites
	std::vector<std::string> _actionBlocks;

	///@brief The selected action
	unsigned char _spriteCounter = 0;

	///@brief Check if thecommand window is active
	bool _commandWindowIsActive = false;
};

#endif // ACTIONBLOCK_H 