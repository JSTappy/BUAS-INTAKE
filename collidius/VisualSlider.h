#ifndef VISUALSLIDER_H 
#define VISUALSLIDER_H

#include "entity.h"

class VisualSlider : public Entity
{
public:
	/// @brief constructor
	VisualSlider(unsigned char playerID);

	/// @brief destructor
	virtual ~VisualSlider();

	/// @brief update method
	/// @param deltaTime
	virtual void Update(float deltaTime); //update

	///@brief Initiate the visual slider properties
	void InitVisuals();

	///@brief Update the slider filling based on time
	///@param float attackCharge, the amount that will be added to the scale
	void UpdateSliderSpriteOnTime(float attackCharge);

	///@brief Update the slider filling based on the amount of key presses or deltatime when holding the key
	///@param float attackCharge, the amount that will be added to the scale
	///@param float attackMax, the amount that will define the slider as filled
	void UpdateSliderSpriteOnKeys(float attackCharge, float attackMax);


private:

	///@brief The players ID, to seperate player 1 from player 2
	unsigned char _playerId;

	///@brief Check when to change the fill sprite to the pure white sprite
	bool _perfectTiming = false;

	///@brief The filling of the visual slider
	Entity* _fill = nullptr;

	///@brief The player key sprite
	Entity* _playerKey = nullptr;

	///@brief The amount of the scale that is filled
	float _fillScaleAmount = 0;

	///@brief The action key sprite for player 1
	std::string _player1Sprite = ("assets/sprites/k.tga");

	///@brief The action key sprite for player 2
	std::string _player2Sprite = ("assets/sprites/j.tga");
};

#endif // VISUALSLIDER_H 