#ifndef UIWINDOW_H 
#define UIWINDOW_H

#include "entity.h"

class UIWindow : public Entity
{
public:
	/// @brief constructor
	UIWindow();

	/// @brief destructor
	virtual ~UIWindow();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	///@brief The health display text of player 1
	Text* playerOneText;

	///@brief The health display text of player 2
	Text* playerTwoText;

	///@brief The health display text of the enemy
	Text* enemyText;

private:
	
};

#endif // UIWINDOW_H 