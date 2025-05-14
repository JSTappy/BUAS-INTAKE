#ifndef COMMANDWINDOW_H 
#define COMMANDWINDOW_H

#include "entity.h"
#include "GameEntity.h"

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

	/// @brief Handle the choosing of actions
	void HandleActionChoosing();

	/// @brief Displays the commands of an action
	/// @param unsigned char action, the type of action being displayed
	void DisplayCommands(unsigned char action);

	/// @brief Get the selected action count
	/// @return _selectedActionCount
	unsigned char GetSelectedAction() { return _selectedActionCount; }

	void CleanUp();

private:

	///@brief Arrow sprite
	Entity* _arrow;

	///@brief vector of actions
	std::vector<Entity*> _actions;

	///@brief The action that is selected
	unsigned char _selectedActionCount;

	///@brief The type of action the action i
	unsigned char _actionType;

	///@brief the text strings to show in battletext from the turnmanager for Attacks
	std::vector<const char*> _attackStrings
	{
		"Move towards the enemy and punch him!",
		"Jump on the enemies head 3 times for maximum damage!",
		"Fire a powerful projectile at the cost of your health!",
		"Rush to the enemy and lower both entities defenses!"
	};

	///@brief the text strings to show in battletext from the turnmanager for Defense actions
	std::vector<const char*> _defendStrings
	{
		"Increase your Defense by 1 point",
		"Set your damage reduction to 15% and skip a turn",
		"Increase your Defense by 20 points but sacrifice 10 power",
		"Set your damage reduction to 50% and skip 3 turns"
	};

	///@brief the text strings to show in battletext from the turnmanager for Items
	std::vector<const char*> _itemStrings
	{
		"Heal 15% of Max HP!",
		"Boost your power at the cost of speed!",
		"Boost your speed at the cost of power!",
		"Trade off 50% HP for twice the power!"
	};

	///@brief Snap the arrow to the entity, as a way to see which command you are selecting
	void SnapArrowToEntity(Entity* entity);

	///@brief The option switching sound when changing options
	Audio::Sound* _switchOptionSound = new Audio::Sound("assets/audio/commandwindowselect.ogg", Audio::Sound::Type::Sound);
};

#endif // COMMANDWINDOW_H 