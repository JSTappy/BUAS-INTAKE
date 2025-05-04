#include "game.h" 

Game::Game()
{
	running = false;
}
Game::~Game()
{

}

void Game::Run(Scene* scene)
{
	// get deltaTime and update camera
	float dt = _renderer.updateDeltaTime();
	scene->camera()->computeViewMatrixFromInput(_renderer.window(), dt);
	_inMan->InputUpdate(_renderer.window());

	this->UpdateEntity(scene, dt);

	// Render the scene
	_renderer.RenderScene(scene);

	if (glfwGetKey(_renderer.window(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(_renderer.window()) == 0)
	{
		running = true;
	}
	else
	{
		running = false;
	}

}
void Game::UpdateEntity(Entity* e, float deltaTime)
{
	e->Update(deltaTime);
	for (Entity* ce : e->GetChildren())
	{
		UpdateEntity(ce, deltaTime);
	}
}