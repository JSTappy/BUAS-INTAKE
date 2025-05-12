#include <iostream>
#include "myscene.h"
#include "TurnManager.h"

MyScene::MyScene() : Scene()
{
	_startTimer = new Timer();
	_startTimer->StartTimer();
	_startTimer->isPlaying = true;
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
	_player1->text = _uiDisplay->playerTextA;
	_player1->UpdateHealthText();
	_player1->position = glm::vec3(400, 300, 0);
	_player1->SetStartPos();
	_player1->AssignActionKey(KEY_Q);
	_layer2->AddChild(_player1);
	gameEntities.push_back(_player1);

	_player2 = new Player(2, 97, 35, 16, 36, 0, 10);
	_player2->SetSprite("assets/sprites/jchar.tga");
	_player2->text = _uiDisplay->playerTextB;
	_player2->UpdateHealthText();
	_player2->position = glm::vec3(250, 500, 0);
	_player2->SetStartPos();
	_player2->AssignActionKey(KEY_E);
	_layer2->AddChild(_player2);
	gameEntities.push_back(_player2);

	_enemy = new Enemy(3, 1063, 30, 15, 60, 0, 10);
	_enemy->SetSprite("assets/sprites/gorilla.tga");
	_enemy->text = _uiDisplay->enemyText;
	_enemy->UpdateHealthText();
	_enemy->position = glm::vec3(950, 360, 0);
	_enemy->SetStartPos();
	_layer2->AddChild(_enemy);
	gameEntities.push_back(_enemy);

	for (size_t i = 0; i < gameEntities.size(); i++)
	{
		TurnManager::Instance()->AddGameEntities(gameEntities[i]);
	}
	TurnManager::Instance()->Init();
	TurnManager::Instance()->battleText = _uiDisplay->text;
}



MyScene::~MyScene()
{

}

void MyScene::Update(float deltaTime)
{
	if (_startTimer->GetSeconds() <= 2 && _startTimer->isPlaying) { return; }
	if (!setupComplete) { TurnManager::Instance()->DecideTurnOrder(); _startTimer->StopTimer();  setupComplete = true; }

	if (GetInput()->GetKeyDown(KEY_Z)) { TurnManager::Instance()->DisplayStats(); }
	if (GetInput()->GetKeyDown(KEY_T))
	{
		displayHitboxes = !displayHitboxes;
		for (size_t i = 0; i < gameEntities.size(); i++)
		{
			gameEntities[i]->ToggleHitboxDisplay(displayHitboxes);
		}
	}
	for (size_t i = 0; i < gameEntities.size(); i++)
	{
		if (gameEntities[i]->health <= 0)
		{
			std::cout << "Game Entity Defeated" << std::endl;
			gameEntities[i]->alive = false;
			_layer2->RemoveChild(gameEntities[i]);
			TurnManager::Instance()->KillEntity(gameEntities[i]);
			delete gameEntities[i];
			gameEntities[i] = nullptr;
			gameEntities.erase(gameEntities.begin() + i);

		}
	}
}