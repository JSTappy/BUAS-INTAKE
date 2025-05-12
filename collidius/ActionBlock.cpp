#include "ActionBlock.h" 

ActionBlock::ActionBlock()
{
	_actionBlocks.push_back(_attackSprite);
	_actionBlocks.push_back(_defendSprite);
	_actionBlocks.push_back(_itemSprite);;
	this->SetSprite(_attackSprite);
	_spriteCounter = 0;
	_commandWindowIsActive = false;
	_commandWindow = nullptr;
}
ActionBlock::~ActionBlock()
{

}

void ActionBlock::Update(float deltaTime)
{
	if (!_commandWindowIsActive) return;
}

void ActionBlock::NextBlock()
{
	if (_spriteCounter >= 2)
	{
		_spriteCounter = 0;
		this->SetSprite(_actionBlocks[_spriteCounter]);
		std::cout << _spriteCounter << std::endl;
		return;
	}
	_spriteCounter++;
	this->SetSprite(_actionBlocks[_spriteCounter]);
	std::cout << _spriteCounter << std::endl;

}

void ActionBlock::PreviousBlock()
{
	if (_spriteCounter <= 0)
	{
		_spriteCounter = 2;
		this->SetSprite(_actionBlocks[_spriteCounter]);
		std::cout << _spriteCounter << std::endl;
		return;
	}
	_spriteCounter--;
	this->SetSprite(_actionBlocks[_spriteCounter]);
	std::cout << _spriteCounter << std::endl;
}

void ActionBlock::SelectBlock()
{
	_commandWindowIsActive = true;
}

void ActionBlock::OpenCommandWindow() 
{
	_commandWindow = new CommandWindow();
	_commandWindow->SetSprite("assets/sprites/commandwindow.tga");
	_commandWindow->scale = glm::vec3(1.5f, 4.5f, 0.0f);
	_commandWindow->position = glm::vec3(128 * (_commandWindow->scale.x / 2), 128 * (_commandWindow->scale.y / 2), 0.0f);
	/*std::cout << "Commandwindow sprite width: " << _commandWindow->sprite->GetWidth() << std::endl;
	std::cout << "Commandwindow sprite height: " << _commandWindow->sprite->GetHeight() << std::endl;*/
	this->AddChild(_commandWindow);
	_commandWindow->DisplayCommands(_spriteCounter);
}

void ActionBlock::CleanUp()
{
	_commandWindow->CleanUp();
	this->RemoveChild(_commandWindow);
	delete _commandWindow;
	_commandWindow = nullptr;
}

glm::vec2 ActionBlock::ConfirmAction()
{
	return glm::vec2(_spriteCounter, _commandWindow->GetSelectedAction());
}
