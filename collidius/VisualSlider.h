#ifndef VISUALSLIDER_H 
#define VISUALSLIDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MyEntity.h"

class VisualSlider : public MyEntity
{
public:
	/// @brief constructor
	VisualSlider(int playerID);

	/// @brief destructor
	virtual ~VisualSlider();

	/// @brief update method
	/// @param deltaTime
	/// @return void
	virtual void Update(float deltaTime); //update

	void InitVisuals();
	void UpdateSliderSpriteOnTime(float attackCharge);
	void UpdateSliderSpriteOnClicks(float attackCharge, float attackMax);
	void SetAttacker(int playerIndex);

	void ClearFill();
private:
	int _playerId;
	bool _perfectTiming;
	MyEntity* _fill;
	MyEntity* _playerKey;

	float _fillScaleAmount = 0.0f;
	std::string _player1Sprite = ("assets/sprites/k.tga");
	std::string _player2Sprite = ("assets/sprites/j.tga");
};

#endif // VISUALSLIDER_H 