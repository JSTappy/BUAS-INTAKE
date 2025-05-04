#include "Player.h" 
#include "TurnManager.h" 

Player::Player(int id, int level, int health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints)
	: GameEntity(id, level, health, power, defense, speed, damageReduction, criticalChance, money, experiencePoints) // Call the GameEntity constructor
{
	_jumpKey = 0;
	_isgrounded = true;
	_shouldFall = false;
	_ascendTimer = new Timer();
	_attackingTimer = new Timer();
	_multipleProjectileTimer = new Timer();
	this->AddChild(_ascendTimer);
	this->AddChild(_attackingTimer);
	this->AddChild(_multipleProjectileTimer);
	_ascendTimer->StopTimer();
	_attackingTimer->StopTimer();
	_multipleProjectileTimer->StopTimer();
	_target = nullptr;
	_actionBlock = nullptr;
	_visualSlider = nullptr;
}

Player::~Player()
{

}

void Player::Update(float deltaTime) 
{
	switch (gameEntityState)
	{
		case idle:
			// EnableJump(deltaTime);
			return;
		case attacking:
			//std::cout << _attackingTimer->GetSeconds() << std::endl;
			switch (_attackType)
			{
				case 0:
					_visualSlider->UpdateSliderSpriteOnTime(_attackingTimer->GetSeconds());
					HandlePunch();
					return;
				case 1:
					if (!_shotsFired)
					{
						HandleProjectileMash(); 
						return;
					}
					if (_projectiles.size() > 0)
					{
						for (int i = 0; i < _projectiles.size(); i++)
						{
							HandleProjectileCollision(_projectiles[i], _target);
						}
						return;
					}
					ResetToIdle();
					return;
				case 2:
					return;
				case 3:
					return;
			}
			return;
		case choosing:
			EnableJump(deltaTime);
			if (choosingAction)
			{
				if (_actionBlock == nullptr)
				{
					_actionBlock = new ActionBlock();
					_actionBlock->position = this->_startPos + glm::vec3(0, -200, 0);
					this->AddChild(_actionBlock);
				}
				if (GetInput()->GetKeyDown(KEY_A) && _isgrounded)
				{
					_actionBlock->PreviousBlock();
				}
				if (GetInput()->GetKeyDown(KEY_D) && _isgrounded)
				{
					_actionBlock->NextBlock();
				}
				if (this->position.y < _actionBlock->position.y + _actionBlock->sprite->GetHeight())
				{
					this->_velocity = glm::vec3(0, 0, 0);
					ReturnToStartPos();
					// Select Action
					_actionBlock->OpenCommandWindow();
					choosingCommand = true;
					choosingAction = false;
				}
				return;
			}
			else if (choosingCommand)
			{
				if (GetInput()->GetKeyDown(Tab))
				{
					ClearHUD();
					choosingAction = true;
					choosingCommand = false;
					return;
				}
				if (this->position.y < _actionBlock->position.y + _actionBlock->sprite->GetHeight())
				{
					this->_velocity = glm::vec3(0, 0, 0);
					HandleAction();
				}
			}
			return;
		case defending:
			EnableJump(deltaTime);
			return;
	}
	
}

void Player::HandleAction()
{
	int actionType = _actionBlock->ConfirmAction().x;
	int actionIndex = _actionBlock->ConfirmAction().y;

	switch (actionType)
	{
		case 0: //attack
			PerformAttack(actionIndex);
			break;
		case 1: //defend
			//increase damage reduction by 30 %
			this->_damageReduction = 0.3f;
			std::cout << "THE DEFENDER" << std::endl;
			ResetToIdle();
			return;
		case 2: //item
			//use item
			break;
		case 3: //special
			//perform special attack
			break;
	}
}

void Player::PerformAttack(int attackLevel) 
{
	_attackType = attackLevel;
	_target = _turnManager->GetRandomEnemy();
	switch (attackLevel)
	{
		case 0:
			PunchAttack(_target);
			return;
		case 1:
			ReturnToStartPos();
			MashProjectileAttack(_target);
			return;
		case 2:
			return;
		case 3:
			return;
	}
}

void Player::EnableJump(float deltaTime) 
{
	if (_ascendTimer->GetSeconds() > 0.20f)
	{
		std::cout << "PEAK" << std::endl;
		std::cout << this->position.y << std::endl;
		_ascendTimer->StopTimer();
		_shouldFall = true;
	}
	if (GetInput()->GetKeyDown(_jumpKey) && _isgrounded)
	{
		_velocity = glm::vec3(0, 0, 0);
		Jump();
		_isgrounded = false;
		_shouldFall = false;
	}
	if (!_isgrounded && _shouldFall)
	{
		_velocity.y += _gravity * deltaTime;
	}
	Move(deltaTime);
	if (this->position.y < 0 && !_isgrounded || this->position.y > _startPos.y && !_isgrounded)
	{
		_isgrounded = true;
		_shouldFall = false;
		_velocity = glm::vec3(0, 0, 0);
		ReturnToStartPos();
		return;
	}
}

void Player::HandlePunch() 
{
	if (GetInput()->GetKeyDown(_jumpKey))
	{
		if (_attackingTimer->GetSeconds() < 1.0f)
		{
			std::cout << "Not Enough but Still Good" << std::endl;
			DealDamage(_target, 0.3);
			ResetToIdle();
			return;
		}
		if (_attackingTimer->GetSeconds() > 1.0f && _attackingTimer->GetSeconds() < 1.5f)
		{
			std::cout << "MAX CHARGE SUCCESS" << std::endl;
			DealDamage(_target, 1);
			ResetToIdle();
			return;
		}
		if (_attackingTimer->GetSeconds() > 1.5f)
		{
			std::cout << "Failed" << std::endl;
			ResetToIdle();
			return;
		}
		completedTurn = true;
		return;
	}
	if (_attackingTimer->GetSeconds() >= 2.0f)
	{
		std::cout << "Did nothing, return to start" << std::endl;
		ResetToIdle();
		return;
	}
}

void Player::HandleProjectileMash()
{
	if (_attackingTimer->GetSeconds() >= 1.5f)
	{
		if (_projectileChargeStored <= 1.0f)
		{
			_projectileChargeStored = 0.0f;
		}
		if (_projectileChargeStored >= 1.0f && _projectileChargeStored < 2.0f)
		{
			_projectileChargeStored = 1.0f;
		}
		if (_projectileChargeStored >= 2.0f && _projectileChargeStored < 3.0f)
		{
			_projectileChargeStored = 2.0f;
		}
		if (_projectileChargeStored >= 3.0f)
		{
			_projectileChargeStored = 3.0f;
		}
		std::cout << "FINAL CHARGE AMOUNT:  " << _projectileChargeStored << std::endl;

		//spawn projectile you fire at the opponent
		Projectile* projectile = new Projectile(_target->GetStartPos(), this->position, this->power);
		this->AddChild(projectile);
		_projectiles.push_back(projectile);
		projectile->SetSprite("assets/sprites/kblast.tga");
		_shotsFired = true;
		return;
	}
	if (GetInput()->GetKeyDown(_jumpKey))
	{
		_projectileChargeStored += _projectileChargeGain;
		_visualSlider->UpdateSliderSpriteOnClicks(_projectileChargeGain, _projectileChargeMax);
		std::cout << "Charging!! " << _projectileChargeStored << std::endl;
	}
}

void Player::HandleProjectileAction()
{
	DealDamage(_target, _projectileChargeStored);
}


void Player::SelectBlock()
{
	_actionBlock->OpenCommandWindow();
}

void Player::BasicAttack(int damage, GameEntity* target)
{
	target->health -= damage; //basic
}

void Player::PunchAttack(GameEntity* target)
{
	ClearHUD();
	this->_velocity = glm::vec3(0, 0, 0);
	this->position = target->position - glm::vec3(target->sprite->GetWidth() + 50.0f, 0, 0);
	InitiateVisualSlider();
	gameEntityState = attacking;
	_attackingTimer->StartOverTimer();
	_target = target;
}

void Player::MashProjectileAttack(GameEntity* target)
{
	ClearHUD();
	this->_velocity = glm::vec3(0, 0, 0);
	gameEntityState = attacking;
	InitiateVisualSlider();
	_attackingTimer->StartOverTimer();
	_target = target;
}

void Player::InitiateVisualSlider()
{
	_visualSlider = new VisualSlider(this->GetID());
	_visualSlider->SetSprite("assets/sprites/slider.tga");
	_visualSlider->position = this->position - glm::vec3(0.0f, 128.0f, 0.0f);
	_visualSlider->scale = glm::vec3(1.0f, 0.5f, 1.0f);
	this->AddChild(_visualSlider);
	_visualSlider->InitVisuals();
}


void Player::AssignJumpKey(int jumpKey)
{
	this->_jumpKey = jumpKey;
}

void Player::ResetToIdle()
{
	ReturnToStartPos();
	_isgrounded = true;
	_shouldFall = false;
	gameEntityState = idle;
	completedTurn = true;
	_projectileChargeStored = 0.0f;
	_shotsFired = false;
	_attackingTimer->StopTimer();
	ClearHUD();
}

void Player::ClearHUD() 
{
	if (_actionBlock != nullptr)
	{
		this->RemoveChild(_actionBlock);
		_actionBlock->CleanUp();
		delete _actionBlock;
		_actionBlock = nullptr;
		//std::cout << "clearing action block" << std::endl;
	}
	if (_visualSlider != nullptr)
	{
		this->RemoveChild(_visualSlider);
		_visualSlider->ClearFill();
		delete _visualSlider;
		_visualSlider = nullptr;
		//std::cout << "clearing slider" << std::endl;
	}
}

void Player::Move(float deltaTime)
{
	this->position += _velocity * deltaTime;
}

void Player::Jump()
{
	_velocity -= glm::vec3(0.0f, 800.0f, 0.0f);
	_ascendTimer->StartOverTimer();
}
