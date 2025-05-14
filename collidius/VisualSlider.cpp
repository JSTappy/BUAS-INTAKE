#include "VisualSlider.h" 

VisualSlider::VisualSlider(unsigned char playerID)
{
	_playerId = playerID;
	InitVisuals();
}

VisualSlider::~VisualSlider()
{
	delete _fill;
	_fill = nullptr;
	delete _playerKey;
	_playerKey = nullptr;
}

void VisualSlider::InitVisuals()
{
	//Create the fill sprite
	_fill = new MyEntity(); 
	_fill->SetSprite("assets/sprites/sliderfill.tga");
	_fill->position = this->position;
	_fill->scale = this->scale;
	_fill->scale.x = 0;
	this->AddChild(_fill);

	//Create the player key sprite
	_playerKey = new MyEntity();
	if (_playerId == 1) { _playerKey->SetSprite(_player1Sprite); } //If player 1, set player1 key sprite to the playerkey sprite
	else { _playerKey->SetSprite(_player2Sprite); } //If player 2, set player2 key sprite to the playerkey sprite
	_playerKey->position = this->position;
	_playerKey->position.y -= 100;
	_playerKey->scale = glm::vec3(2, 2, 0);
	this->AddChild(_playerKey);
}

void VisualSlider::Update(float deltaTime)
{
	//Make sure the fill and the playerkey follow the slider
	_fill->position = this->position;
	_playerKey->position.x = this->position.x;
	_playerKey->position.y = this->position.y - 100;
}

void VisualSlider::UpdateSliderSpriteOnTime(float attackCharge)
{
	if (_perfectTiming) return; //No need to update the slidersprite when its full and white
	if (_fill->scale.x >= 1) //If the scale is larger than 1, perfect timing is true
	{
		//Make the sprite white
		_fill->SetSprite("assets/sprites/purewhite.tga");
		_perfectTiming = true;
		return;
	}
	_fill->scale.x = attackCharge; //Scale the fill with attackCharge
}

void VisualSlider::UpdateSliderSpriteOnKeys(float attackCharge, float attackMax)
{
	if (_perfectTiming) return; //No need to update the slidersprite when its full and white

	_fillScaleAmount = attackMax;
	_fill->scale.x += attackCharge / _fillScaleAmount; //Scale the fill with attackCharge divided by the _fillScaleAmount

	if (_fill->scale.x >= 1) //If the scale is larger than 1, perfect timing is true
	{
		//Make the sprite white
		_fill->SetSprite("assets/sprites/purewhite.tga");
		_perfectTiming = true;
		_fillScaleAmount = 0;
		return;
	}
}