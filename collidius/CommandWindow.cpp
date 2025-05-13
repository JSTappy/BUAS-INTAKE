#include "CommandWindow.h" 
#include "TurnManager.h" 

CommandWindow::CommandWindow()
{
	_arrow = new MyEntity();
	_arrow->SetSprite("assets/sprites/arrow.tga");
	_arrow->position = this->position;
	this->AddChild(_arrow);
	_selectedActionCount = 0;
}

CommandWindow::~CommandWindow()
{

}

void CommandWindow::Update(float deltaTime)
{
	HandleActionChoosing();
}

void CommandWindow::HandleActionChoosing() 
{
	float randomPitch = 0.75f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
	if (GetInput()->GetKeyDown(KEY_W))
	{
		_switchOptionSound->setPitch(randomPitch);
		_switchOptionSound->replay();
		if (_selectedActionCount <= 0)
		{
			_selectedActionCount = 3;
			SnapArrowToEntity(_actions[_selectedActionCount]);
			return;
		}
		_selectedActionCount--;
		SnapArrowToEntity(_actions[_selectedActionCount]);

	}
	if (GetInput()->GetKeyDown(KEY_S))
	{
		_switchOptionSound->setPitch(randomPitch);
		_switchOptionSound->replay();
		if (_selectedActionCount >= 3)
		{
			_selectedActionCount = 0;
			SnapArrowToEntity(_actions[_selectedActionCount]);
			return;
		}
		_selectedActionCount++;
		SnapArrowToEntity(_actions[_selectedActionCount]);

	}
}

void CommandWindow::DisplayCommands(unsigned char action)
{
	_actionType = action;
	unsigned short ypos = 78;
	for (int i = 0; i < 4; i++)
	{
		MyEntity* e = new MyEntity();

		//Might become text later
		//e->SetSprite("assets/sprites/rgba.tga");
		e->text = new Text();
		switch (action)
		{
			case 0: //attack
				e->text->text = "ATTACK " + std::to_string(i + 1);
				break;
			case 1: //defend
				e->text->text = "DEFEND " + std::to_string(i + 1);
				break;
			case 2: //item
				e->text->text = "ITEM " + std::to_string(i + 1);
				break;
		}
		e->position.x = this->position.x;
		e->position.y = ypos;
		e->scale = glm::vec3(1, 1, 0);
		this->AddChild(e);
		_actions.push_back(e);
		e->textComponents.push_back(e->text);
		ypos += 144;
	}
	_selectedActionCount = 0;
	SnapArrowToEntity(_actions[0]);
	/*switch (_actions.size())
	{
		case 0:
			std::cout << "Atleast something 1" << std::endl;
			break;
		case 1:
			std::cout << "Atleast something 2" << std::endl;
			break;
		case 2:
			std::cout << "Atleast something 3" << std::endl;
			break;
		case 3:
			std::cout << "Atleast something 4" << std::endl;
			break;
	}*/
}

void CommandWindow::SnapArrowToEntity(MyEntity* entity) 
{
	_arrow->position = entity->position;
	_arrow->position.x = entity->position.x + 150;
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

void CommandWindow::CleanUp()
{
	for (size_t i = 0; i < _actions.size(); i++)
	{
		this->RemoveChild(_actions[i]);
		_actions.erase(_actions.begin() + i);
		delete _actions[i];
		_actions[i] = nullptr;
		//std::cout << "Deleting Action" << std::endl;
	}
	//std::cout << "Actions before clearing: " << _actions.size() << std::endl;
	_actions.clear();
	//std::cout << "Actions after clearing: " << _actions.size() << std::endl;
	this->RemoveChild(_arrow);
	delete _arrow;
	_arrow = nullptr;
}