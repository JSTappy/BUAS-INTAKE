#include "CommandWindow.h" 

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
	if (GetInput()->GetKeyDown(KEY_W))
	{
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

void CommandWindow::DisplayCommands(int action)
{
	int ypos = 78;
	for (int i = 0; i < 4; i++)
	{
		MyEntity* e = new MyEntity();

		//Might become text later
		e->SetSprite("assets/sprites/rgba.tga");
		e->position.x = this->position.x;
		e->position.y = ypos;
		e->scale = glm::vec3(1.0f, 1.0f, 0.0f);
		this->AddChild(e);
		_actions.push_back(e);
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
}

void CommandWindow::ShowActionProperty() 
{
	//TODO show text in the panel on the bottom screen
}

void CommandWindow::CleanUp()
{
	for (int i = 0; i < _actions.size(); i++)
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