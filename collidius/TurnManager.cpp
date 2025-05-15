#include "TurnManager.h" 

TurnManager::TurnManager()
{
	//Add the timer to the turnmanager
	_waitingTimer = new Timer();
	_waitingTimer->StopTimer();
}
TurnManager::~TurnManager()
{
	//Clear the lists, because this is only called when exiting the game
	_gameEntities.clear();
	_players.clear();

	//Delete and dereference the timer
	delete _waitingTimer;
	_waitingTimer = nullptr;

	//Dereference these because they will be deleted by the entity class
	battleText = nullptr;
	_nextInTurn = nullptr;
}

void TurnManager::Update(float deltaTime)
{
	if (stopUpdating)return;
	//Update the waitingtimer because it does not automatically update like in Entity
	_waitingTimer->Update(deltaTime);

	if (_nextInTurn == nullptr) return; //If there is no next in turn, return
	if (!_nextInTurn->completedTurn) return; //If the one currently in turn has not completed their turn yet, return
	if (!_timerStarted)
	{
		//If the timer has not started, start the timer
		_waitingTimer->StartTimer();
		_timerStarted = true;
	}
	if (_waitingTimer->GetSeconds() <= 0.75f) return; //Wait for the waiting timer to reach 0.75 seconds
	GiveTurnToNext(); //Allow the next one in turn to be chosen
}

void TurnManager::DecideTurnOrder()
{
	unsigned char lastSpeed = 0; //Check the speeds
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		//Check if the next game entities' speed is higher than the previous one
		if (_gameEntities[i]->speed > lastSpeed)
		{
			//Set the next in turn to the game entity with the highest speed
			_nextInTurn = _gameEntities[i];
		}
		lastSpeed = _gameEntities[i]->speed; //Set the last speed to the speed of the game entity checked
		_gameEntities[i]->completedTurn = false; //set their turn completion to false
	}

	//After the for loop, set the next in turns state to choosing
	_nextInTurn->gameEntityState = _nextInTurn->choosing;
	battleText->text = "Choosing Action";
}

void TurnManager::DecideNextInTurn()
{
	unsigned char completedTurnEntities = 0; //See who has completed their turn
	unsigned char lastSpeed = 0; //Check the speeds
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		//If the game entity has completed its turn, add 1 to the counter and continue the for loop
		if (_gameEntities[i]->completedTurn)
		{
			completedTurnEntities++;
			continue;
		}
		//If the above was not true, and the entities speed was higher than the last speed, set that entity to the next in turn
		if (_gameEntities[i]->speed > lastSpeed)
		{
			_nextInTurn = _gameEntities[i];
		}
		lastSpeed = _gameEntities[i]->speed; //Set the last speed to the speed of the game entity checked
	}

	//After the for loop, if all game entities have completed their turn, decide a new turn order
	if (completedTurnEntities == _gameEntities.size())
	{
		DecideTurnOrder();
		return;
	}

	//Set the next in turn to choosing
	_nextInTurn->gameEntityState = _nextInTurn->choosing;
	_nextInTurn->choosingCommand = false;
	_nextInTurn->choosingAction = true;
}

void TurnManager::GiveTurnToNext()
{
	//If the players are not grounded, return because we dont want them inside the action blocks
	for (size_t i = 0; i < _players.size(); i++) { if (!_players[i]->GetIsGrounded()) return; }

	//Stop the timer
	_waitingTimer->StopTimer();

	//Set everyone to idle
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		_gameEntities[i]->gameEntityState = _gameEntities[i]->idle;
	}

	//Complete the turn and teleport the next in turn to its start position. next dereference it and set the timer started to false
	_nextInTurn->completedTurn = true;
	_nextInTurn = nullptr;
	_timerStarted = false;

	//Decide the next in turn
	DecideNextInTurn();
}


void TurnManager::AddGameEntities(GameEntity* ge)
{
	//Set everyone to idle
	ge->gameEntityState = ge->idle;
	_gameEntities.push_back(ge);

	//Check if the game entity is a player or an enemy and set them correctly
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		if (Player* player = dynamic_cast<Player*>(_gameEntities[i]))
		{
			_players.push_back(player);
		}
		if (Enemy* enemy = dynamic_cast<Enemy*>(_gameEntities[i]))
		{
			_enemy = enemy;
		}
	}
}


void TurnManager::KillEntity(GameEntity* ge)
{
	//Dereference the gameEntity
	_gameEntities.erase(std::remove(_gameEntities.begin(), _gameEntities.end(), ge), _gameEntities.end());

	//Check if its a player
	if (Player* player = dynamic_cast<Player*>(ge))
	{
		_players.erase(std::remove(_players.begin(), _players.end(), player), _players.end());
	}
	//No need to remove the enemy from a list since there is none, the enemy deletion gets done in the scene
}

Player* TurnManager::GetRandomPlayer()
{
	//Return a random player if there are players
	if (_players.size() > 0)
	{
		unsigned char randomPlayer = rand() % _players.size();
		return _players[randomPlayer];
	}

	//Return nothing otherwise
	return nullptr;
}

TurnManager* TurnManager::Instance()
{
	//Instantiate this class if it hasnt been done already
	if (TurnManager::_instance == nullptr)
	{
		//Create a new TurnManager
		_instance = new TurnManager();
	}

	//Return the instance
	return _instance;
}

//Define and initialize the instance
TurnManager* TurnManager::_instance = nullptr;
