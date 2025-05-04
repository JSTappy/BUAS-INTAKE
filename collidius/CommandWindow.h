#ifndef COMMANDWINDOW_H 
#define COMMANDWINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "entity.h"
#include "GameEntity.h"
#include "myentity.h"

class CommandWindow : public Entity
{
public:
	/// @brief constructor
	CommandWindow();

	/// @brief destructor
	virtual ~CommandWindow();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	void HandleActionChoosing();
	void DisplayCommands(int action);

	int GetSelectedAction() { return _selectedActionCount; }

	void CleanUp();

private:
	MyEntity* _arrow;
	std::vector<MyEntity*> _actions;
	int _selectedActionCount;

	void SnapArrowToEntity(MyEntity* entity);
	void ShowActionProperty();
};

#endif // COMMANDWINDOW_H 