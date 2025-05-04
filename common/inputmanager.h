#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <map>
#include <string>
#include "sprite.h"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/// @brief The Inputmanager class handles getting the keyboard keys.
class InputManager
{
public:
	/// @brief constructor
	InputManager();

	/// @brief destructor
	virtual ~InputManager();

	/// @brief public input instance
	static InputManager* input();

	/// @brief for updating the input
	/// @param w the window you want to register the input from
	/// @return w
	void InputUpdate(GLFWwindow* w);

	/// @brief GetKey for looking if the key keeps being pressed
	/// @param k the key currently being held down
	/// @return k
	bool GetKey(int k);

	/// @brief GetKeyDown for looking if the key has been pressed once
	/// @param k the key that has been pressed once
	/// @return k
	bool GetKeyDown(int k);

	/// @brief GetKeyUp for looking if the key has been released
	/// @param k the key that has released
	/// @return k
	bool GetKeyUp(int k);

	
	int GetWindowWidth() { return _windowWidth; }
	int GetWindowHeight() { return _windowHeight; }


private:
	/// @brief Window the input is registered in
	GLFWwindow* _window;

	/// @brief instance of InputManager
	static InputManager* _instance;

	/// @brief for checking what keys are being pressed
	/// @param key the key that is supposed to be handled
	/// return void
	void _handleKey(int key);

	int _windowWidth; 
	int _windowHeight;

	/// @brief keys
	std::map<int, bool> _keys;

	/// @brief keysDown
	std::map<int, bool> _keysDown;

	/// @brief keysUp
	std::map<int, bool> _keysUp;


};

#endif // !INPUTMANAGER_H