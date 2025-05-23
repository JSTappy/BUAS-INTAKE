#ifndef MYSCENE_H
#define MYSCENE_H

#include "scene.h"
#include "game.h"
#include "entity.h"
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
	
private:

	///@brief Complete the tutorial if it has not been completed already
	void CompleteTutorial();

	///@brief Handle the tutorial with dialogue and music
	void HandleTutorial();

	///@brief Complete the game in a certain way based on how many players are left
	void CompleteGame(char status);

	///@brief Check what images to show in the tutorial
	void CheckToShowImages(char textCount);

	///@brief Handle the tutorial with dialogue and music
	char _playersLeft = 2;

	///@brief Check if the game is completed, if true, exit game
	bool _gameComplete = false;

	//Image to show with both the tutorial as well as the end screen
	Entity* _gameStateImage;
	
	///@brief The background
	Entity* _layer1;

	///@brief The entity containing the game entities and projectiles
	Entity* _layer2;

	///@brief The entity containing the UI at the bottom of the screen
	Entity* _layer3;

	///@brief The first player, Lee
	Player* _player1;

	///@brief The second player, Cole
	Player* _player2;

	///@brief The enemy, Sean
	Enemy* _enemy;

	///@brief The window at the bottom of the screen with the HP and battle text
	UIWindow* _uiDisplay;

	///@brief Timer for closing the game after winning
	Timer* _endTimer;

	///@brief the background music that will loop
	Audio::Sound* _bgm = new Audio::Sound("assets/audio/bgm.ogg", Audio::Sound::Type::Music);

	///@brief the intro to the background music of 2.71f seconds
	Audio::Sound* _intro = new Audio::Sound("assets/audio/intro.ogg", Audio::Sound::Type::Sound);

	///@brief Check if the hiboxes should be displayed
	bool _displayHitboxes = false;

	///@brief Check if the game has been set up
	bool setupComplete;

	///@brief The amount of seconds the timer should reach before moving on to the next index in the _introDialogue
	float _textinterval = 0;

	///@brief the index of the introdialogue
	int _textcount = 0;

	///@brief the list of gameEntities
	std::vector<GameEntity*> _gameEntities;

	///@brief The dialogue that will play as a tutorial
	std::vector<const char*> _introDialogue
	{
		"*Press [SPACEBAR] to see the next text message",
		"'Its just us two left, huh Cole?'",
		"'I guess you're right... Lee.'",
		"'This gorilla... Sean is something else.'",
		"'Alright, we can choose to attack, defend or use an item.'",
		"*You press the Q key to jump as the player at the top",
		"*You press the E key to jump as the player at the bottom",
		"'So we jump to hit the action block and choose what we do?'",
		"'Exactly! when we do, we can choose from 4 actions.'",
		"*Use the WASD keys to navigate through the menus",
		"'We deal massive damage if we use our actions right.'",
		"*IMPORTANT: Keep looking at this window when you can",
		"*This window will provide info about the attack execution",
		"'Be careful though! the gorilla attacks by throwing bananas.'",
		"'How do we know where he is gonna throw them?'",
		"'I think his eyes say alot...'",
		"'We can jump over them or choose to stay still.'",
		"'Let's get him Cole! We humans have no limits!'",
		"'Damn right Lee! Let's charge together at full power!'",
		"..."
	};
};

#endif /* MYSCENE_H */