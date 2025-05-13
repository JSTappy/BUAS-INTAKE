#ifndef MYSCENE_H
#define MYSCENE_H

#include "scene.h"
#include "game.h"
#include "entity.h"
#include "myentity.h"
#include "text.h"

#include "GameEntity.h"
#include "Player.h"
#include "Enemy.h"
#include "UIWindow.h"

#include <Audio/Sound.hpp>

/// @brief The MyScene implementation.
class MyScene : public Scene
{
public:
	/// @brief Constructor
	MyScene();

	/// @brief Destructor
	virtual ~MyScene();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime);

	bool displayHitboxes = false;
	
private:
	Player* _player1;
	Player* _player2;
	Enemy* _enemy;

	UIWindow* _uiDisplay;
	MyEntity* _battleText;
	MyEntity* _layer1;
	MyEntity* _layer2;
	MyEntity* _layer3;

	Timer* _startTimer;

	bool setupComplete;

	Audio::Sound* _bgm = new Audio::Sound("assets/audio/bgm.ogg", Audio::Sound::Type::Music);
	Audio::Sound* _intro = new Audio::Sound("assets/audio/intro.ogg", Audio::Sound::Type::Sound);

	bool _skippedTutorial;

	void CompleteTutorial();
	void HandleTutorial();
	float _textinterval = 0;
	int _textcount = 0;
	std::vector<GameEntity*> _gameEntities;
	std::vector<const char*> _textStrings
	{
		"...",
		"'Its just us two left, huh Cole?'",
		"'I guess you're right... Lee.'",
		"'This gorilla... Sean is something else'",
		"'Alright, we can choose to attack, defend or use an item'",
		"'We jump to hit the action block and choose what we do right?'",
		"'Exactly! when we do, we can choose from 4 actions.'",
		"'We deal massive damage if we use our actions'",
		"'Let's get him Cole! We humans have no limits!'",
		"'Damn right Lee! Let's charge together at full power!'",
	};
};

#endif /* MYSCENE_H */