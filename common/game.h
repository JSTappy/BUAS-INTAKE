#ifndef GAME_H 
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "config.h"
#include "renderer.h"
#include "camera.h"
#include "inputmanager.h"

class Game
{
public:
	/// @brief constructor
	Game();

	/// @brief destructor
	virtual ~Game();

	/// @brief method for running the scene
	/// @param scene the scene
	/// @return void
	void Run(Scene* scene);

	/// @brief for instantiating the input
	InputManager* _inMan = InputManager::input();

	/// @brief Update Entity method
	/// @param e, deltaTime the entity you want to update, deltaTime
	/// @return void
	void UpdateEntity(Entity* e, float deltaTime);

	/// @brief this is for getting the private boolean 'runnning'
	/// @return running
	bool IsRunning() { return running; }

	Renderer GetRenderer() { return _renderer; };


private:
	/// @brief renderer, we only need renderer in game
	Renderer _renderer;

	/// @brief private boolean to check if the game is running
	bool running;
};

#endif // GAME_H 