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
	_layer1->position = glm::vec3(WIDTH / 2, HEIGHT / 2, 0.0f);
	_layer2 = new MyEntity();
	_layer3 = new MyEntity();

	this->AddChild(_layer1);
	this->AddChild(_layer2);
	this->AddChild(_layer3);

	_uiDisplay = new UIWindow();
	_uiDisplay->SetSprite("assets/sprites/uimockup.tga");
	_uiDisplay->position = glm::vec3(WIDTH / 2, HEIGHT - 64.0f, 0.0f);
	_layer3->AddChild(_uiDisplay);
	
	_player1 = new Player(1, 1, 55, 40, 9, 44, 0.0f, 2, 200, 0);
	_player1->SetSprite("assets/sprites/kchar.tga");
	_player1->text = _uiDisplay->playerTextA;
	_player1->UpdateHealthText();
	_player1->position = glm::vec3(400.0f, 300.0f, 0.0f);
	_player1->SetStartPos();
	_player1->AssignActionKey(KEY_K);
	_layer2->AddChild(_player1);
	gameEntities.push_back(_player1);

	_player2 = new Player(2, 1, 97, 35, 16, 36, 0.0f, 2, 200, 0);
	_player2->SetSprite("assets/sprites/jchar.tga");
	_player2->text = _uiDisplay->playerTextB;
	_player2->UpdateHealthText();
	_player2->position = glm::vec3(250.0f, 500.0f, 0.0f);
	_player2->SetStartPos();
	_player2->AssignActionKey(KEY_J);
	_layer2->AddChild(_player2);
	gameEntities.push_back(_player2);


	_enemy = new Enemy(3, 1, 1000, 30, 15, 60, 0.0f, 2, 500, 16);
	_enemy->SetSprite("assets/sprites/gorilla.tga");
	_enemy->text = _uiDisplay->enemyText;
	_enemy->UpdateHealthText();
	_enemy->position = glm::vec3(950.0f, 360.0f, 0.0f);
	_enemy->SetStartPos();
	_layer2->AddChild(_enemy);
	gameEntities.push_back(_enemy);


	for (int i = 0; i < gameEntities.size(); i++)
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
	if (_startTimer->GetSeconds() <= 2.0f && _startTimer->isPlaying) { return; }
	if (!setupComplete) { TurnManager::Instance()->DecideTurnOrder(); _startTimer->StopTimer();  setupComplete = true; }

	if (GetInput()->GetKeyDown(KEY_Q)) { TurnManager::Instance()->DisplayStats(); }
	if (GetInput()->GetKeyDown(KEY_T))
	{
		displayHitboxes = !displayHitboxes;
		for (int i = 0; i < gameEntities.size(); i++)
		{
			gameEntities[i]->ToggleHitboxDisplay(displayHitboxes);
		}
	}
	for (int i = 0; i < gameEntities.size(); i++)
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