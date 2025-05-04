#include "inputmanager.h"
#include "config.h"
#include <iostream>

InputManager::InputManager()
{
	_window = nullptr;

	_windowWidth = 0; 
	_windowHeight = 0; 


}

InputManager::~InputManager()
{

}

void InputManager::InputUpdate(GLFWwindow* w)
{
	_window = w;

	//  window size
	glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);


	// 32-97 = ' ' to '`'
	for (unsigned int i = 32; i < 97; i++) {
		_handleKey(i);
	}
	// Func + arrows + esc, etc
	for (unsigned int i = 255; i < GLFW_KEY_LAST; i++) {
		_handleKey(i);
	}
}

bool InputManager::GetKey(int k)
{
	return _keys[k];
}

bool InputManager::GetKeyDown(int k)
{
	return _keysDown[k];
}

bool InputManager::GetKeyUp(int k)
{
	return _keysUp[k];
}

void InputManager::_handleKey(int key)
{
	if (glfwGetKey(_window, key) == GLFW_PRESS) { //If key is pressed
		if (_keys[key] == false) { //if the key has not already been pressed, set keys to true and set keydown to true, else keysdown will be false 
			_keys[key] = true;
			_keysDown[key] = true;
		}
		else {
			_keysDown[key] = false;
		}
	}
	if (glfwGetKey(_window, key) == GLFW_RELEASE) { //if you release the key, release key is true but it will not always be true so set it to false if you release the key
		if (_keys[key] == true) {
			_keys[key] = false;
			_keysUp[key] = true;
		}
		else {
			_keysUp[key] = false;
		}
	}
}

InputManager* InputManager::input() //for instansiating the input manager (only happens once)
{
	if (InputManager::_instance == nullptr)
	{
		_instance = new InputManager();
	}
	return _instance;
}

InputManager* InputManager::_instance = nullptr;
