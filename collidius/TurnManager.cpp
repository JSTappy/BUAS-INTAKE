#include "TurnManager.h" 

TurnManager::TurnManager()
{
	_nextInTurn = nullptr;
	_waitingTimer = new Timer();
	_waitingTimer->StopTimer();
	_timerStarted = false;
}
TurnManager::~TurnManager()
{

}

void TurnManager::Init()
{
	for (int i = 0; i < _gameEntities.size(); i++)
	{
		if (Player* player = dynamic_cast<Player*>(_gameEntities[i]))
		{
			_players.push_back(player);
		}
		if (Enemy* enemy = dynamic_cast<Enemy*>(_gameEntities[i]))
		{
			_enemies.push_back(enemy);
		}
	}
	std::cout << "amount of Players: " << _players.size() << std::endl;
	std::cout << "amount of Enemies: " << _enemies.size() << std::endl;
}

void TurnManager::Update(float deltaTime)
{
	_waitingTimer->Update(deltaTime);
	if (_nextInTurn == nullptr) return;
	if (!_nextInTurn->completedTurn) return;
	if (!_timerStarted)
	{
		_waitingTimer->StartTimer();
		_timerStarted = true;
	}
	GiveTurnToNext();
}

void TurnManager::DecideTurnOrder()
{
	int lastSpeed = 0;
	for (int i = 0; i < _gameEntities.size(); i++)
	{
		if (_gameEntities[i]->speed > lastSpeed)
		{
			_nextInTurn = _gameEntities[i];
			//std::cout << "Current Fastest:" << std::endl;
			//std::cout << _gameEntities[i]->GetID()<< std::endl;
		}
		lastSpeed = _gameEntities[i]->speed;
		_gameEntities[i]->completedTurn = false;

	}
	_nextInTurn->gameEntityState = _nextInTurn->choosing;
	battleText->text = "Next in turn ID: " + std::to_string(_nextInTurn->GetID());
	//std::cout << "First Attacker:" << std::endl;
	//std::cout << _nextInTurn->GetID() << std::endl;
}

void TurnManager::DecideNextInTurn()
{
	int completedTurnEntities = 0;
	int lastSpeed = 0;
	for (int i = 0; i < _gameEntities.size(); i++)
	{
		if (_gameEntities[i]->completedTurn)
		{
			completedTurnEntities++;
			continue;
		}
		if (_gameEntities[i]->speed > lastSpeed)
		{
			_nextInTurn = _gameEntities[i];
		}
		lastSpeed = _gameEntities[i]->speed;
	}
	if (completedTurnEntities == _gameEntities.size())
	{
		DecideTurnOrder();
		return;
	}
	battleText->text = "Entity that is choosing action: " + std::to_string(_nextInTurn->GetID());
	_nextInTurn->gameEntityState = _nextInTurn->choosing;
	_nextInTurn->choosingCommand = false;
	_nextInTurn->choosingAction = true;
}

void TurnManager::GiveTurnToNext()
{
	if (_waitingTimer->GetSeconds() <= 0.75f) return;
	for (int i = 0; i < _players.size(); i++) { if (!_players[i]->GetIsGrounded()) return; }
	_waitingTimer->StopTimer();
	for (int i = 0; i < _gameEntities.size(); i++) 
	{
		_gameEntities[i]->gameEntityState = _gameEntities[i]->idle;
	}
	_nextInTurn->completedTurn = true;
	_nextInTurn->TeleportToPosition(_nextInTurn->GetStartPos());
	_nextInTurn = nullptr;
	_timerStarted = false;
	DecideNextInTurn();
}


void TurnManager::AddGameEntities(GameEntity* ge)
{
	ge->gameEntityState = ge->idle;
	_gameEntities.push_back(ge);
}

void TurnManager::DisplayStats()
{
	battleText->text = "Game Entity ID: " + std::to_string(_nextInTurn->GetID());
	std::cout << "######################################################################### " << std::endl;
	std::cout << "Game Entity ID: " << _nextInTurn->GetID() << std::endl;
	std::cout << "Level: " << _nextInTurn->GetLevel() << std::endl;
	std::cout << "Health: " << _nextInTurn->health << std::endl;
	std::cout << "Power: " << _nextInTurn->power << std::endl;
	std::cout << "Defense: " << _nextInTurn->defense << std::endl;
	std::cout << "Speed: " << _nextInTurn->speed << std::endl;
	std::cout << "Damage reduction: " << _nextInTurn->GetDamageReduction() * 100 << "%" << std::endl;
	std::cout << "Critical chance: " << _nextInTurn->GetCriticalChance() << "%" << std::endl;
	std::cout << "######################################################################### " << std::endl;
}


void TurnManager::KillEntity(GameEntity* ge)
{
	_gameEntities.erase(std::remove(_gameEntities.begin(), _gameEntities.end(), ge), _gameEntities.end());

	if (Player* player = dynamic_cast<Player*>(ge))
	{
		std::cout << "Player Killed" << std::endl;
		_players.erase(std::remove(_players.begin(), _players.end(), player), _players.end());
	}

	if (Enemy* enemy = dynamic_cast<Enemy*>(ge))
	{
		std::cout << "Enemy Killed" << std::endl;
		_enemies.erase(std::remove(_enemies.begin(), _enemies.end(), enemy), _enemies.end());
	}
}

Player* TurnManager::GetRandomPlayer()
{
	if (_players.size() != 0)
	{
		int randomPlayer = rand() % _players.size();
		std::cout << randomPlayer << std::endl;
		return _players[randomPlayer];
	}
}

Enemy* TurnManager::GetRandomEnemy()
{
	if (_enemies.size() != 0)
	{
		int randomEnemy = rand() % _enemies.size();
		std::cout << randomEnemy << std::endl;
		return _enemies[randomEnemy];
	}
}

TurnManager* TurnManager::Instance() //for instansiating the input manager (only happens once)
{
	if (TurnManager::_instance == nullptr)
	{
		_instance = new TurnManager();
	}
	return _instance;
}

TurnManager* TurnManager::_instance = nullptr;
