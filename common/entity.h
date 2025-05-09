#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <glm/glm.hpp>
#include "sprite.h"
#include "inputmanager.h"
#include "keyconfig.h"
#include "text.h"
#include <iostream>

class Entity 
{
public:
	/// @brief constructor
	Entity();

	/// @brief destructor
	virtual ~Entity();

	/// @brief update function
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime) = 0;

	/// @brief _input getter
	/// @return _input
	InputManager* GetInput() { return _input; };

	/// @brief method for adding a child
	/// @param c (child)
	/// @return void
	void AddChild(Entity* c);

	/// @brief method for removing a child
	/// @param c (child)
	/// @return void
	void RemoveChild(Entity* c);

	/// @brief children getter
	/// @return _children
	std::vector<Entity*> GetChildren() { return _children; };

	/// @brief all children getter
	/// @return all childeren
	std::vector<Entity*> GetAllChildren();

	/// @brief set sprite method
	/// @param sprite
	/// @return void
	void SetSprite(const std::string& imagepath);

	/// @brief sprite of the entity 
	Sprite* sprite;

	/// @brief position of the entity
	glm::vec3 position;

	/// @brief rotstion of the entity
	float rotation;

	/// @brief scale of the entity
	glm::vec3 scale;

	Text* text;

	std::vector<Text*> textComponents;


	void SetTextPosition(Text* t, glm::vec3 textPosition);
private: 

	/// @brief parent of the entity
	Entity* _parent;

	/// @brief list of children 
	std::vector<Entity*> _children;


	/// @brief instance of InputManager
	InputManager* _input = InputManager::input();

};

#endif /* ENTITY_H */
