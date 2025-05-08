#include <iostream>
#include "myscene.h"

MyScene::MyScene() : Scene()
{
	this->AddChild(turnManager);

	_layer1 = new MyEntity();
	_layer1->SetSprite("assets/sprites/bg.tga");
	_layer1->position = glm::vec3(WIDTH / 2, HEIGHT / 2, 0.0f);
	_layer2 = new MyEntity();
	_layer3 = new MyEntity();

	this->AddChild(_layer1);
	this->AddChild(_layer2);
	this->AddChild(_layer3);
	
	_player1 = new Player(1, 1, 50, 40, 9, 44, 0.0f, 2, 200, 0);
	_player1->SetSprite("assets/sprites/kchar.tga");
	_player1->position = glm::vec3(400.0f, 300.0f, 0.0f);
	_player1->SetStartPos();
	_player1->SetTextPosition(glm::vec3(16, 720 - 72, 0));
	_player1->AssignActionKey(KEY_K);
	_layer2->AddChild(_player1);
	gameEntities.push_back(_player1);


	_player2 = new Player(2, 1, 70, 35, 16, 36, 0.0f, 2, 200, 0);
	_player2->SetSprite("assets/sprites/jchar.tga");
	_player2->position = glm::vec3(250.0f, 500.0f, 0.0f);
	_player2->SetTextPosition(glm::vec3(16, 720 - 32, 0));
	_player2->SetStartPos();
	_player2->AssignActionKey(KEY_J);
	_layer2->AddChild(_player2);
	gameEntities.push_back(_player2);

	_enemy = new Enemy(3, 1, 500, 30, 15, 60, 0.0f, 2, 500, 16);
	_enemy->SetSprite("assets/sprites/gorilla.tga");
	_enemy->position = glm::vec3(950.0f, 360.0f, 0.0f);
	_enemy->SetStartPos();
	_layer2->AddChild(_enemy);
	gameEntities.push_back(_enemy);

	_uiDisplay = new MyEntity();
	_uiDisplay->SetSprite("assets/sprites/uimockup.tga");
	_uiDisplay->position = glm::vec3(WIDTH / 2, HEIGHT - 64.0f, 0.0f);
	_uiDisplay->text = new Text();
	_uiDisplay->text->moveWithEntity = true;
	_uiDisplay->text->pivot = glm::vec2(128.0f, 0.0f);
	_uiDisplay->text->text = "This is where you will see info such as things but does this scale like screensize?";
	_uiDisplay->text->color = WHITE;
	_layer1->AddChild(_uiDisplay);

	for (int i = 0; i < gameEntities.size(); i++)
	{
		turnManager->AddGameEntities(gameEntities[i]);
	}
	turnManager->Init();
	turnManager->battleText = _uiDisplay->text;
	turnManager->DecideTurnOrder();
}



MyScene::~MyScene()
{

}

void MyScene::Update(float deltaTime)
{
	if (GetInput()->GetKeyDown(KEY_Q))
	{
		turnManager->DisplayStats();
	}
	if (GetInput()->GetKeyDown(KEY_T))
	{
		for (int i = 0; i < gameEntities.size(); i++)
		{
			gameEntities[i]->ToggleHitboxDisplay(displayHitboxes);
		}
		displayHitboxes = !displayHitboxes;
	}
	for (int i = 0; i < gameEntities.size(); i++)
	{
		if (gameEntities[i]->health <= 0)
		{
			std::cout << "Game Entity Defeated" << std::endl;
			gameEntities[i]->alive = false;
			_layer2->RemoveChild(gameEntities[i]);
			turnManager->KillEntity(gameEntities[i]);
			delete gameEntities[i];
			gameEntities[i] = nullptr;
			gameEntities.erase(gameEntities.begin() + i);

		}
	}
}