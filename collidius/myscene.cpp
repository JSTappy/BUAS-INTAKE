#include <iostream>
#include "myscene.h"
#include "TurnManager.h"

MyScene::MyScene() : Scene()
{
	//Initiate the timer
	_startTimer = new Timer();
	this->AddChild(_startTimer);

	//Add the layers
	_layer1 = new MyEntity();

	//Set the background sprite
	_layer1->SetSprite("assets/sprites/bg.tga");
	_layer1->position = glm::vec3(WIDTH / 2, HEIGHT / 2, 0);

	_layer2 = new MyEntity();
	_layer3 = new MyEntity();

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

	//Start the timer
	_startTimer->StartTimer();
	_startTimer->isPlaying = true;
}

MyScene::~MyScene()
{

}

void MyScene::Update(float deltaTime)
{
	if (!setupComplete) { HandleTutorial(); return; } //Handle the tutorial if it has not been completed yet
	if (GetInput()->GetKeyDown(KEY_Z)) { TurnManager::Instance()->DisplayStats(); } //Display stats if the Z key is pressed
	if (GetInput()->GetKeyDown(KEY_T)) //Display the hitboxes if the T key is pressed
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
			std::cout << "Game Entity Defeated" << std::endl;
			_gameEntities[i]->alive = false;
			_layer2->RemoveChild(_gameEntities[i]);
			TurnManager::Instance()->KillEntity(_gameEntities[i]); //Remove the game entity from the turnmanagers list as well
			delete _gameEntities[i]; //Delete it here
			_gameEntities[i] = nullptr;
			_gameEntities.erase(_gameEntities.begin() + i); //remove it from the list

			//TODO complete game screen
		}
	}

	//Play the music if it is not playing yet
	if (_bgm->isPlaying())return;
	_bgm->setLooping(true);
	_bgm->setVolume(0.5f);
	_bgm->play();
}
void MyScene::HandleTutorial()
{
	if (GetInput()->GetKeyDown(SPACE)) { CompleteTutorial(); return; } //Skip the tutorial when pressing Space Key

	/*	If the textinterval is below or equal to the amount of seconds the start timer
		And the textcount is still smaller than the intro dialogue strings size
		And if the text interval is below 24.39f

		Switch to the next intro dialogue string
	*/
	if (_textinterval <= _startTimer->GetSeconds() && _textcount < _introDialogue.size() && _textinterval < 24.39f)
	{
		_textcount++;
		TurnManager::Instance()->battleText->text = _introDialogue[_textcount];
		_textinterval += 2.71f;
		return;
	}
	
	//If the text interval is smaller than 24.39f, return because the if statement above this does not get called every frame
	if (_textinterval < 24.39f)return;

	//Play the intro once the last string is on screen and the time is below the interval
	if (_startTimer->GetSeconds() <= 24.39f)
	{
		//Play the intro if it isnt playing already
		if (_intro->isPlaying())return;
		_intro->play();
		_intro->setVolume(0.8f);
		return;
	}
	if (!setupComplete)CompleteTutorial(); //Complete the tutorial if the timer exceeds the 24.39f threshold
}
void MyScene::CompleteTutorial() 
{
	//Start the game
	TurnManager::Instance()->StartGame(); 
	_startTimer->StopTimer(); //Stop the timer
	_startTimer->isPlaying = false;
	if (_intro->isPlaying())_intro->stop(); //Stop the music
	setupComplete = true;
}