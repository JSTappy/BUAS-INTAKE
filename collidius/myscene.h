#ifndef MYSCENE_H
#define MYSCENE_H

#include "scene.h"
#include "game.h"
#include "entity.h"
#include "myentity.h"
#include "text.h"

#include "GameEntity.h"
#include "Player.h"
#include "Enemy.h"
#include "UIWindow.h"

/// @brief The MyScene implementation.
class MyScene : public Scene
{
public:
	/// @brief Constructor
	MyScene();

	/// @brief Destructor
	virtual ~MyScene();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime);

	bool displayHitboxes = false;
	
private:
	Player* _player1;
	Player* _player2;
	Enemy* _enemy;

	UIWindow* _uiDisplay;
	MyEntity* _battleText;
	MyEntity* _layer1;
	MyEntity* _layer2;
	MyEntity* _layer3;

	Timer* _startTimer;

	bool setupComplete;

	std::vector<GameEntity*> gameEntities;
};

#endif /* MYSCENE_H */