#ifndef TURNMANAGER_H 
#define TURNMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "entity.h"
#include "Player.h"
#include "Enemy.h"

class TurnManager : public Entity
{
public:
	/// @brief constructor
	TurnManager();
	/// @brief destructor
	virtual ~TurnManager();

	void Init();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	void AddGameEntities(GameEntity* ge);
	void DisplayStats();
	void KillEntity(GameEntity* ge);

	void DecideTurnOrder();
	void DecideNextInTurn();
	void GiveTurnToNext();

	std::vector<Player*> GetPlayerList() { return _players; }
	std::vector<Enemy*> GetEnemyList() { return _enemies; }

	Player* GetRandomPlayer();
	Enemy* GetRandomEnemy();

	Text* battleText;
private:
	Timer* _waitingTimer;
	std::vector<GameEntity*> _gameEntities;
	std::vector<Player*> _players;
	std::vector<Enemy*> _enemies;

	bool _timerStarted;
	GameEntity* _nextInTurn;
};

#endif // TURNMANAGER_H 