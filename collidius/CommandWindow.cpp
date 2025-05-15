#include "CommandWindow.h" 
#include "TurnManager.h" 

CommandWindow::CommandWindow()
{
	//Create the arrow
	_arrow = new Entity();
	_arrow->SetSprite("assets/sprites/arrow.tga");
	_arrow->position = this->position;
	this->AddChild(_arrow);


	_selectedActionCount = 0;
	_switchOptionSound->setVolume(0.7f); //Set volume lower
}

CommandWindow::~CommandWindow()
{
	//Remove the actions
	for (size_t i = 0; i < _actions.size(); i++)
	{
		this->RemoveChild(_actions[i]);
		_actions.erase(_actions.begin() + i);
		delete _actions[i];
		_actions[i] = nullptr;
	}
	_actions.clear();

	//Destroy the arrow
	this->RemoveChild(_arrow);
	delete _arrow;
	_arrow = nullptr;
}

void CommandWindow::Update(float deltaTime)
{
	HandleActionChoosing();
}

void CommandWindow::HandleActionChoosing() 
{
	//When pressing W, Go up one entity in the actions list
	if (GetInput()->GetKeyDown(KEY_W))
	{
		_switchOptionSound->replay(); //Replay sound
		if (_selectedActionCount <= 0) //If at the minimum of the actions list
		{
			_selectedActionCount = 3; //Set it to the max of the actions list
			SnapArrowToEntity(_actions[_selectedActionCount]); //Snap the arrow to the entity
			return;
		}
		_selectedActionCount--;
		SnapArrowToEntity(_actions[_selectedActionCount]); //Snap the arrow to the entity

	}

	//When pressing S, Go down one entity in the actions list
	if (GetInput()->GetKeyDown(KEY_S))
	{
		_switchOptionSound->replay(); //Replay sound
		if (_selectedActionCount >= 3) //If at the maximum of the actions list
		{
			_selectedActionCount = 0; //Set it to the minimum of the actions list
			SnapArrowToEntity(_actions[_selectedActionCount]); //Snap the arrow to the entity
			return;
		}
		_selectedActionCount++;
		SnapArrowToEntity(_actions[_selectedActionCount]); //Snap the arrow to the entity

	}
}

void CommandWindow::DisplayCommands(unsigned char action)
{
	_actionType = action;
	unsigned short ypos = 78; //Y-position displacement

	//Create 4 'commands' (entities) for the action chosen
	for (int i = 0; i < 4; i++)
	{
		//Create Entities
		Entity* e = new Entity();
		e->text = new Text(); //Set up text
		switch (action)
		{
			case 0: //Action is Attacking
				e->text->text = "ATTACK " + std::to_string(i + 1);
				break;
			case 1: //Action is Defending
				e->text->text = "DEFEND " + std::to_string(i + 1);
				break;
			case 2: //Action is using Items
				e->text->text = "ITEM " + std::to_string(i + 1);
				break;
		}

		//Complete the entity set up
		e->position.x = this->position.x;
		e->position.y = ypos; //The displacement
		e->scale = glm::vec3(1, 1, 0);
		this->AddChild(e);
		_actions.push_back(e);
		e->textComponents.push_back(e->text); //Make sure the text gets rendered
		ypos += 144; //Displace the ypos even further
	}
	_selectedActionCount = 0;
	SnapArrowToEntity(_actions[0]); //Snap the arrow to the top of the list
}

void CommandWindow::SnapArrowToEntity(Entity* entity) 
{
	//Set the arrow to the entities position, with an offset
	_arrow->position = entity->position;
	_arrow->position.x = entity->position.x + 150;

	//Accordingly change the battle text based on what action and command is selected
	switch (_actionType)
	{
		case 0:
			TurnManager::Instance()->battleText->text = _attackStrings[_selectedActionCount];
			break;
		case 1:
			TurnManager::Instance()->battleText->text = _defendStrings[_selectedActionCount];
			break;
		case 2:
			TurnManager::Instance()->battleText->text = _itemStrings[_selectedActionCount];
			break;
	}
}