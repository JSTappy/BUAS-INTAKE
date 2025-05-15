#include <iostream>
#include "myscene.h"
#include "TurnManager.h"

MyScene::MyScene() : Scene()
{

	//Add the layers
	_layer1 = new Entity();

	//Set the background sprite
	_layer1->SetSprite("assets/sprites/bg.tga");
	_layer1->position = glm::vec3(WIDTH / 2, HEIGHT / 2, 0);

	_layer2 = new Entity();
	_layer3 = new Entity();

	this->AddChild(_layer1);
	this->AddChild(_layer2);
	this->AddChild(_layer3);

	//Add the UIWindow
	_uiDisplay = new UIWindow();
	_uiDisplay->SetSprite("assets/sprites/uimockup.tga");
	_uiDisplay->position = glm::vec3(WIDTH / 2, HEIGHT - 64, 0.0f);
	_layer3->AddChild(_uiDisplay);
	
	//Add Player 1
	_player1 = new Player(1, 74, 40, 9, 44, 0, 10);
	_player1->SetSprite("assets/sprites/kchar.tga");
	_player1->text = _uiDisplay->playerOneText; //Set the player one text to the ui player one text so it can render on layer 3
	_player1->UpdateHealthText();
	_player1->position = glm::vec3(400, 300, 0);
	_player1->SetStartPos();
	_player1->AssignActionKey(KEY_Q); //The action key used to perform actions
	_layer2->AddChild(_player1);
	_gameEntities.push_back(_player1);

	//Add Player 1
	_player2 = new Player(2, 97, 35, 16, 36, 0, 10);
	_player2->SetSprite("assets/sprites/jchar.tga");
	_player2->text = _uiDisplay->playerTwoText; //Set the player two text to the ui player two text so it can render on layer 3
	_player2->UpdateHealthText();
	_player2->position = glm::vec3(250, 500, 0);
	_player2->SetStartPos();
	_player2->AssignActionKey(KEY_E); //The action key used to perform actions
	_layer2->AddChild(_player2);
	_gameEntities.push_back(_player2);

	//Add the Enemy
	_enemy = new Enemy(3, 1063, 30, 15, 60, 0, 10);
	_enemy->SetSprite("assets/sprites/gorilla.tga");
	_enemy->text = _uiDisplay->enemyText; //Set the enemy text to the ui enemy text so it can render on layer 3
	_enemy->UpdateHealthText();
	_enemy->position = glm::vec3(950, 360, 0);
	_enemy->SetStartPos();
	_layer2->AddChild(_enemy);
	_gameEntities.push_back(_enemy);

	//Add these game entities to the turnmanager game entity list
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		TurnManager::Instance()->AddGameEntities(_gameEntities[i]);
	}
	TurnManager::Instance()->battleText = _uiDisplay->text; //Set turnmanager battle text to uiwindow battle text so it can render on layer 3
	TurnManager::Instance()->battleText->text = _introDialogue[0];

	//Initiate the timer
	_endTimer = new Timer();
	this->AddChild(_endTimer);
	_endTimer->StopTimer();
}

MyScene::~MyScene()
{
	//Clean up game entities
	for (GameEntity* entity : _gameEntities)
	{
		if (entity)
		{
			_layer2->RemoveChild(entity);
			TurnManager::Instance()->KillEntity(entity);
			delete entity;
		}
	}
	_gameEntities.clear();

	//Destroy sprite on layer 3
	_layer3->RemoveChild(_gameStateImage);
	delete _gameStateImage;
	_gameStateImage = nullptr;
	
	//Destroy layers
	this->RemoveChild(_layer1);
	this->RemoveChild(_layer2);
	this->RemoveChild(_layer3);

	delete _layer1; 
	delete _layer2; 
	delete _layer3;

	_layer1 = nullptr;
	_layer2 = nullptr; 
	_layer3 = nullptr;

	//Destroy UI display
	delete _uiDisplay; 
	_uiDisplay = nullptr;

	//Destroy game entity pointers
	_player1 = nullptr;
	_player2 = nullptr;
	_enemy = nullptr;

	//Destroy timer
	this->RemoveChild(_endTimer);
	delete _endTimer;
	_endTimer = nullptr;

	//Destroy audio
	delete _bgm; _bgm = nullptr; 
	delete _intro; _intro = nullptr; 
}

void MyScene::Update(float deltaTime)
{
	if (_endTimer->GetSeconds() <= 10 && _endTimer->isPlaying)return;
	if (_gameComplete)
	{
		shouldCloseGame = true;
		return;
	}
	if (!setupComplete) { HandleTutorial(); return; } //Handle the tutorial if it has not been completed yet

	//Display the hitboxes if the T key is pressed
	if (GetInput()->GetKeyDown(KEY_T)) 
	{
		_displayHitboxes = !_displayHitboxes;
		for (size_t i = 0; i < _gameEntities.size(); i++)
		{
			_gameEntities[i]->ToggleHitboxDisplay(_displayHitboxes); //Toggle the hitboxes
		}
	}

	//Check if a game entity has died
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		//If the health of the game entity is 0, kill it
		if (_gameEntities[i]->health <= 0)
		{
			_gameEntities[i]->alive = false;
			_layer2->RemoveChild(_gameEntities[i]);
			TurnManager::Instance()->KillEntity(_gameEntities[i]); //Remove the game entity from the turnmanagers list as well
			switch (_gameEntities[i]->GetID())
			{
				case 1: //Player 1 died
					_playersLeft--;
					if (_playersLeft < 1)
					{
						CompleteGame(0);
						return;
					}
					break;
				case 2: //Player 2 died
					_playersLeft--;
					if (_playersLeft < 1)
					{
						CompleteGame(0);
						return;
					}
					break;
				case 3: //Enemy died
					CompleteGame(_playersLeft);
					return;
			}
			delete _gameEntities[i]; //Delete it here
			_gameEntities[i] = nullptr;
			_gameEntities.erase(_gameEntities.begin() + i); //remove it from the list
		}
	}

	//Play the music if it is not playing yet
	if (_bgm->isPlaying())return;
	_bgm->setLooping(true);
	_bgm->setVolume(0.5f);
	_bgm->play();
}

void MyScene::CheckToShowImages(char textCount)
{
	if (textCount == 4)
	{
		_gameStateImage = new Entity();
		_layer3->AddChild(_gameStateImage);
		_gameStateImage->SetSprite("assets/sprites/tutorialslide1.tga");
		_gameStateImage->position = glm::vec3(WIDTH / 2, 294.0f, 0);
	}
	if (textCount == 8)
	{
		_gameStateImage->SetSprite("assets/sprites/tutorialslide2.tga");
	}
	if (textCount == 10)
	{
		_gameStateImage->SetSprite("assets/sprites/tutorialslide3.tga");
	}
	if (textCount == 13)
	{
		_gameStateImage->SetSprite("assets/sprites/tutorialslide4.tga");
	}
	if (textCount == 17)
	{
		_layer3->RemoveChild(_gameStateImage);
		delete _gameStateImage;
		_gameStateImage = nullptr;
	}
}

void MyScene::CompleteGame(char status)
{
	_gameStateImage = new Entity();
	_layer3->AddChild(_gameStateImage);
	_gameStateImage->SetSprite("assets/sprites/endingslide.tga");
	_gameStateImage->position = glm::vec3(WIDTH / 2, 294.0f, 0);
	_gameComplete = true;
	switch (status)
	{
		case 0: //Enemy wins
			TurnManager::Instance()->battleText->text = "Sean the gorilla has defeated everyone! he is stronger after all!";
			break;
		case 1: //Single player wins
			for (size_t i = 0; i < _gameEntities.size(); i++)
			{
				if (_gameEntities[i]->GetID() == 1) //Player 1 wins
				{
					TurnManager::Instance()->battleText->text = "Lee is the last one standing! But Cole was not so lucky";
					break;
				}
				//Player 2 wins
				TurnManager::Instance()->battleText->text = "Cole Has won! But Lee was not so lucky";
				break;
			}
			break;
		case 2: //Players win
			TurnManager::Instance()->battleText->text = "The humans truly have no limits! They won!";
			break;
	}
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		_gameEntities[i]->alive = false;
		_layer2->RemoveChild(_gameEntities[i]);
		TurnManager::Instance()->KillEntity(_gameEntities[i]); //Remove the game entity from the turnmanagers list as well
		delete _gameEntities[i]; //Delete it here
		_gameEntities[i] = nullptr;
		_gameEntities.erase(_gameEntities.begin() + i); //remove it from the list
	}
	TurnManager::Instance()->stopUpdating = true;
	_endTimer->StartOverTimer();
	_endTimer->isPlaying = true;
}

void MyScene::CheckCompletion()
{


}
void MyScene::HandleTutorial()
{
	if (_textcount >= _introDialogue.size() - 1 && !setupComplete) 
	{
		CompleteTutorial();
		return;
	}
	if (GetInput()->GetKeyDown(SPACE))
	{
		_textcount++;
		CheckToShowImages(_textcount);
		TurnManager::Instance()->battleText->text = _introDialogue[_textcount];
	}
}
void MyScene::CompleteTutorial() 
{
	//Start the game
	TurnManager::Instance()->StartGame(); 
	setupComplete = true;
}