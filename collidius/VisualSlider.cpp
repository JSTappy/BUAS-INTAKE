#include "VisualSlider.h" 

VisualSlider::VisualSlider(unsigned char playerID)
{
	_playerId = playerID;
	InitVisuals();
}

VisualSlider::~VisualSlider()
{
	
}

void VisualSlider::InitVisuals()
{
	_fill = new MyEntity();
	_fill->SetSprite("assets/sprites/sliderfill.tga");
	_fill->position = this->position;
	_fill->scale = this->scale;
	_fill->scale.x = 0;
	this->AddChild(_fill);
	_playerKey = new MyEntity();
	if (_playerId == 1)
	{
		_playerKey->SetSprite(_player1Sprite);
	}
	else
	{
		_playerKey->SetSprite(_player2Sprite);
	}
	_playerKey->position = this->position;
	_playerKey->position.y -= 100;
	_playerKey->scale = glm::vec3(2, 2, 0);
	this->AddChild(_playerKey);
}

void VisualSlider::ClearFill()
{
	this->RemoveChild(_fill);
	delete _fill;
	_fill = nullptr;
	this->RemoveChild(_playerKey);
	delete _playerKey;
	_playerKey = nullptr;
}

void VisualSlider::Update(float deltaTime)
{
	_fill->position = this->position;
	_playerKey->position.x = this->position.x;
	_playerKey->position.y = this->position.y - 100;
}

void VisualSlider::UpdateSliderSpriteOnTime(float attackCharge)
{
	if (_perfectTiming) return;
	if (_fill->scale.x >= 1)
	{
		//make sprite white
		_fill->SetSprite("assets/sprites/purewhite.tga");
		_perfectTiming = true;
		return;
	}
	//make the _fill sprite scale from x0 to x1 or more according to the attackingTimer
	_fill->scale.x = attackCharge;


	//BONUS Make the fill scale from left to right, my guess is that it has to do with matrices that I do not want to touch upon this moment
}

void VisualSlider::UpdateSliderSpriteOnClicks(float attackCharge, float attackMax)
{
	if (_perfectTiming) return;
	if (_fillScaleAmount == 0) 
	{
		//with the player attack it will be 3
		_fillScaleAmount = attackMax;
		return;
	}
	//if the max = 3 and the attackcharge is 0.25, it should take 12 charges for the bar to be full
	
	//add the attackcharge to the scale
	_fill->scale.x += attackCharge / _fillScaleAmount;
	if (_fill->scale.x >= 1)
	{
		//make sprite white
		_fill->SetSprite("assets/sprites/purewhite.tga");
		_perfectTiming = true;
		_fillScaleAmount = 0;
		return;
	}
}