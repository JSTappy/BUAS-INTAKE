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
	float dt = _renderer.UpdateDeltaTime();
	scene->camera()->computeViewMatrixFromInput(_renderer.GetWindow(), dt);
	_inMan->InputUpdate(_renderer.GetWindow());
	_turnMan->Update(dt);

	this->UpdateEntity(scene, dt);

	// Render the scene
	_renderer.RenderScene(scene);

	if (!scene->shouldCloseGame && glfwWindowShouldClose(_renderer.GetWindow()) == 0)
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