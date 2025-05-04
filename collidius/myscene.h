#ifndef MYSCENE_H
#define MYSCENE_H

#include "scene.h"
#include "game.h"
#include "entity.h"
#include "myentity.h"
#include "GameEntity.h"
#include "Player.h"
#include "Enemy.h"
#include "TurnManager.h"

/// @brief The MyScene implementation.
class MyScene : public Scene
{
public:
	/// @brief Constructor
	MyScene();

	/// @brief Destructor
	virtual ~MyScene();

	TurnManager* turnManager = new TurnManager();
	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime);
	
private:
	Player* _player1;
	Player* _player2;
	Enemy* _enemy;

	MyEntity* _uiDisplay;
	MyEntity* _layer1;
	MyEntity* _layer2;
	MyEntity* _layer3;

	std::vector<GameEntity*> gameEntities;
};

#endif /* MYSCENE_H */