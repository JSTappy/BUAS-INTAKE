#include "entity.h"

	Entity::Entity()
	{
		//set parent and child to nullptr
		_parent = nullptr;
		sprite = nullptr;

		//position rotation scale
		position = glm::vec3(0, 0, 0);
		rotation = 0;
		scale = glm::vec3(1, 1, 1);
	}

	Entity::~Entity()
	{
		//set the parent and child to nullptr
		_parent = nullptr;
		sprite = nullptr;

		for (Text* t : textComponents)
		{
			delete t;
			t = nullptr;
		}
		textComponents.clear();
	}

	void Entity::AddChild(Entity* c) //Add child method, if the parent is not a nullptr Remove the child from its old parent and make 'this' the new parent
	{
		if (c->_parent != nullptr)
		{
			c->_parent->RemoveChild(c);
		}
		_children.push_back(c);
		c->_parent = this;
	}

	std::vector<Entity*> Entity::GetAllChildren()
	{
		//make a temporary list
		std::vector<Entity*> result;

		//loop through children
		for (Entity* child : _children)
		{
			//add them to the results
			result.push_back(child);

			//new list for all children
			std::vector<Entity*> childDescendants = child->GetAllChildren();

			//order the list correctly
			if (child->GetAllChildren().size() != 0)
			{
				result.insert(result.end(), childDescendants.begin(), childDescendants.end());
			}
		}

		//return the result
		return result;
	}


	void Entity::RemoveChild(Entity* c)  //removing a child from your entity
	{
		for (size_t i = 0; i < _children.size(); i++)
		{
			if (_children[i] == c)
			{
				_children.erase(_children.begin() + i);
				return;
			}
		}
	}

	void Entity::SetSprite(const std::string& imagepath) //Adding a sprite to your entity
	{
		if (sprite != nullptr)
		{
			sprite = nullptr; //in case of switching sprites, dont delete because of the resourcemanager
		}
		sprite = new Sprite(imagepath);
	}

	void Entity::SetTextPosition(Text* t, glm::vec3 textPosition)
	{
		t->moveWithEntity = false;
		t->centered = false;
		t->position = textPosition;
	}