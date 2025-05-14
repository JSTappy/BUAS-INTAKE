#ifndef TURNMANAGER_H 
#define TURNMANAGER_H


#include "GameEntity.h"

#include "Player.h"
#include "Enemy.h"

///@brief The class that checks which entities' turn it is, who is next and fills in the battle text at the bottom of the screen
class TurnManager
{
public:
	/// @brief constructor
	TurnManager();

	/// @brief destructor
	virtual ~TurnManager();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	///@brief Adds the Game Entities to the list and checks if they are polayers or enemies
	///@param GameEntity* for the GameEntity that gets added
	void AddGameEntities(GameEntity* ge);

	///@brief Displays the _nextInTurn's stats like health power etc.
	void DisplayStats();

	///@brief Dereference the entity and remove it from the correct list(s)
	///@param GameEntity* for the GameEntity that gets Dereferenced
	void KillEntity(GameEntity* ge);

	///@brief Decide the turn order from the scene after the tutorial
	void StartGame() { if (!_gameStarted) DecideTurnOrder(); _gameStarted = true; }

	///@brief The public instance of this class, this can be used for public functions
	static TurnManager* Instance();

	///@brief Gets a random player from the player list
	Player* GetRandomPlayer();

	///@brief Gets the enemy
	///@return _enemy
	Enemy* GetEnemy() { return _enemy; }

	///@brief The text at the bottom of the screen that shows you instructions, action functions and more over the state of the game
	Text* battleText = nullptr;
private:

	///@brief Will decide the order in which entity can perform an action based on their speeds
	void DecideTurnOrder();

	///@brief Checks who is next in the turn based on speed and who already had their turn
	void DecideNextInTurn();

	///@brief Waits for a bit and then gives the turn to the one who is next in the turn order
	void GiveTurnToNext();

	///@brief The timer that makes you wait after an entities turn has been completed
	Timer* _waitingTimer;

	///@brief list of the game entities present in the game
	std::vector<GameEntity*> _gameEntities;

	///@brief list of the players present in the game
	std::vector<Player*> _players;

	//@brief the enemy
	Enemy* _enemy = nullptr;

	///@brief The instance of this class
	static TurnManager* _instance;

	///@brief The game entity who is next in turn
	GameEntity* _nextInTurn = nullptr;

	///@brief check if the timer has started
	bool _timerStarted = false;

	///@brief check if the game has started
	bool _gameStarted = false;

};

#endif // TURNMANAGER_H 