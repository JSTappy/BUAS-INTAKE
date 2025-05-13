#include <iostream>
#include "myscene.h"
#include "TurnManager.h"

MyScene::MyScene() : Scene()
{


	_startTimer = new Timer();
	this->AddChild(_startTimer);
	_layer1 = new MyEntity();
	_layer1->SetSprite("assets/sprites/bg.tga");
	_layer1->position = glm::vec3(WIDTH / 2, HEIGHT / 2, 0);
	_layer2 = new MyEntity();
	_layer3 = new MyEntity();

	this->AddChild(_layer1);
	this->AddChild(_layer2);
	this->AddChild(_layer3);

	_uiDisplay = new UIWindow();
	_uiDisplay->SetSprite("assets/sprites/uimockup.tga");
	_uiDisplay->position = glm::vec3(WIDTH / 2, HEIGHT - 64, 0.0f);
	_layer3->AddChild(_uiDisplay);
	
	_player1 = new Player(1, 74, 40, 9, 44, 0, 10);
	_player1->SetSprite("assets/sprites/kchar.tga");
	_player1->text = _uiDisplay->playerOneText;
	_player1->UpdateHealthText();
	_player1->position = glm::vec3(400, 300, 0);
	_player1->SetStartPos();
	_player1->AssignActionKey(KEY_Q);
	_layer2->AddChild(_player1);
	_gameEntities.push_back(_player1);

	_player2 = new Player(2, 97, 35, 16, 36, 0, 10);
	_player2->SetSprite("assets/sprites/jchar.tga");
	_player2->text = _uiDisplay->playerTwoText;
	_player2->UpdateHealthText();
	_player2->position = glm::vec3(250, 500, 0);
	_player2->SetStartPos();
	_player2->AssignActionKey(KEY_E);
	_layer2->AddChild(_player2);
	_gameEntities.push_back(_player2);

	_enemy = new Enemy(3, 1063, 30, 15, 60, 0, 10);
	_enemy->SetSprite("assets/sprites/gorilla.tga");
	_enemy->text = _uiDisplay->enemyText;
	_enemy->UpdateHealthText();
	_enemy->position = glm::vec3(950, 360, 0);
	_enemy->SetStartPos();
	_layer2->AddChild(_enemy);
	_gameEntities.push_back(_enemy);

	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		TurnManager::Instance()->AddGameEntities(_gameEntities[i]);
	}
	TurnManager::Instance()->Init();
	TurnManager::Instance()->battleText = _uiDisplay->text;
	TurnManager::Instance()->battleText->text = _textStrings[0];
	_startTimer->StartTimer();
	_startTimer->isPlaying = true;
}



MyScene::~MyScene()
{

}

void MyScene::Update(float deltaTime)
{
	if (!setupComplete)
	{
		HandleTutorial();
		return;
	}
	if (GetInput()->GetKeyDown(KEY_Z)) { TurnManager::Instance()->DisplayStats(); }
	if (GetInput()->GetKeyDown(KEY_T))
	{
		displayHitboxes = !displayHitboxes;
		for (size_t i = 0; i < _gameEntities.size(); i++)
		{
			_gameEntities[i]->ToggleHitboxDisplay(displayHitboxes);
		}
	}
	for (size_t i = 0; i < _gameEntities.size(); i++)
	{
		if (_gameEntities[i]->health <= 0)
		{
			std::cout << "Game Entity Defeated" << std::endl;
			_gameEntities[i]->alive = false;
			_layer2->RemoveChild(_gameEntities[i]);
			TurnManager::Instance()->KillEntity(_gameEntities[i]);
			delete _gameEntities[i];
			_gameEntities[i] = nullptr;
			_gameEntities.erase(_gameEntities.begin() + i);

		}
	}
	if (_bgm->isPlaying())return;
	_bgm->setLooping(true);
	_bgm->setVolume(0.5f);
	_bgm->play();
}
void MyScene::HandleTutorial()
{
	if (GetInput()->GetKeyDown(SPACE))
	{
		CompleteTutorial();
		return;
	}
	if (_textinterval <= _startTimer->GetSeconds() && _textcount < _textStrings.size() && _textinterval < 24.39f)
	{
		_textcount++;
		TurnManager::Instance()->battleText->text = _textStrings[_textcount];
		_textinterval += 2.71f;
		return;
	}
	if (_textinterval < 24.39f)return;
	if (_startTimer->GetSeconds() <= 24.4f && _startTimer->isPlaying)
	{
		if (_intro->isPlaying())return;
		_intro->play();
		_intro->setVolume(0.8f);
		return;
	}
	if (!setupComplete) { CompleteTutorial(); }
}
void MyScene::CompleteTutorial() 
{
	TurnManager::Instance()->DecideTurnOrder(); 
	_startTimer->StopTimer();  
	_startTimer->isPlaying = false;
	if (_intro->isPlaying()) { _intro->stop(); }
	setupComplete = true;
}