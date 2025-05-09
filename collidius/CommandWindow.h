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
	int _actionType;

	std::vector<const char*> _attackStrings
	{
		"This attack deals decent damage, make sure to time it right!",
		"Jump on the enemies head 3 times for maximum damage!",
		"This attack fires a projectile at the cost of your health...",
		"This attack lowers you and your opponents defense"
	};
	std::vector<const char*> _defendStrings
	{
		"DR + 30 %",
		"BB",
		"BBB",
		"BBBB"
	};
	std::vector<const char*> _itemStrings
	{
		"C",
		"CC",
		"CCC",
		"CCCC"
	};
	std::vector<const char*> _specialStrings
	{
		"D",
		"DD",
		"DDD",
		"DDDD"
	};

	void SnapArrowToEntity(MyEntity* entity);
	void ShowActionProperty();
};

#endif // COMMANDWINDOW_H 