#ifndef UIWINDOW_H 
#define UIWINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "entity.h"
#include "myentity.h"

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

	Text* playerTextA;
	Text* playerTextB;
	Text* enemyText;

private:
	
};

#endif // UIWINDOW_H 