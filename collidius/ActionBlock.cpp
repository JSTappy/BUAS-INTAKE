#include "ActionBlock.h" 

ActionBlock::ActionBlock()
{
	//Add the strings to the action blocks list
	_actionBlocks.push_back(_attackSprite);
	_actionBlocks.push_back(_defendSprite);
	_actionBlocks.push_back(_itemSprite);;

	//Set the first sprite to the attack sprite
	this->SetSprite(_attackSprite);

	_commandWindow = nullptr;
}
ActionBlock::~ActionBlock()
{
	this->RemoveChild(_commandWindow);
	delete _commandWindow;
	_commandWindow = nullptr;
}

void ActionBlock::NextBlock()
{
	//If the spritecounter is the maximum, go to the minimum
	if (_spriteCounter >= 2)
	{
		_spriteCounter = 0;
		this->SetSprite(_actionBlocks[_spriteCounter]); //Set the correct sprite based off of the sprite counter
		std::cout << _spriteCounter << std::endl;
		return;
	}
	_spriteCounter++;
	this->SetSprite(_actionBlocks[_spriteCounter]); //Set the correct sprite based off of the sprite counter
}

void ActionBlock::PreviousBlock()
{
	//If the spritecounter is the minimum, go to the max
	if (_spriteCounter <= 0)
	{
		_spriteCounter = 2;
		this->SetSprite(_actionBlocks[_spriteCounter]); //Set the correct sprite based off of the sprite counter
		std::cout << _spriteCounter << std::endl;
		return;
	}
	_spriteCounter--;
	this->SetSprite(_actionBlocks[_spriteCounter]); //Set the correct sprite based off of the sprite counter
}

void ActionBlock::SelectBlock()
{
	_commandWindowIsActive = true;
}

void ActionBlock::OpenCommandWindow() 
{
	//Create the command window
	_commandWindow = new CommandWindow();
	_commandWindow->SetSprite("assets/sprites/commandwindow.tga");
	_commandWindow->scale = glm::vec3(1.5f, 4.5f, 0.0f);
	_commandWindow->position = glm::vec3(128 * (_commandWindow->scale.x / 2), 128 * (_commandWindow->scale.y / 2), 0.0f);
	this->AddChild(_commandWindow);
	_commandWindow->DisplayCommands(_spriteCounter);
}
