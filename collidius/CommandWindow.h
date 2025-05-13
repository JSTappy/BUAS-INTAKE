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
	void DisplayCommands(unsigned char action);

	unsigned char GetSelectedAction() { return _selectedActionCount; }

	void CleanUp();

private:
	MyEntity* _arrow;
	std::vector<MyEntity*> _actions;

	unsigned char _selectedActionCount;
	unsigned char _actionType;

	std::vector<const char*> _attackStrings
	{
		"Move towards the enemy and punch him!",
		"Jump on the enemies head 3 times for maximum damage!",
		"Fire a powerful projectile at the cost of your health!",
		"Rush to the enemy and lower both entities defenses!"
	};
	std::vector<const char*> _defendStrings
	{
		"Increase your Defense by 1 point",
		"Set your damage reduction to 15% and skip a turn",
		"Increase your Defense by 20 points but sacrifice 10 power",
		"Set your damage reduction to 50% and skip 3 turns"
	};
	std::vector<const char*> _itemStrings
	{
		"Heal 15% of Max HP!",
		"Boost your power at the cost of speed!",
		"Boost your speed at the cost of power!",
		"Trade off 50% HP for twice the power!"
	};

	void SnapArrowToEntity(MyEntity* entity);

	Audio::Sound* _switchOptionSound = new Audio::Sound("assets/audio/commandwindowselect.ogg", Audio::Sound::Type::Sound);
};

#endif // COMMANDWINDOW_H 