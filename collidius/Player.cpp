#include "Player.h" 
#include "TurnManager.h" 

Player::Player(int id, int level, int health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints)
	: GameEntity(id, level, health, power, defense, speed, damageReduction, criticalChance, money, experiencePoints) // Call the GameEntity constructor
{
	_actionKey = 0;
	_isgrounded = true;
	_shouldFall = false;
	_ascendTimer = new Timer();
	_attackingTimer = new Timer();
	_multipleProjectileTimer = new Timer();
	_waitingTimer = new Timer();
	this->AddChild(_waitingTimer);
	this->AddChild(_attackingTimer);
	this->AddChild(_multipleProjectileTimer);
	this->AddChild(_ascendTimer);
	_ascendTimer->StopTimer();
	_attackingTimer->StopTimer();
	_multipleProjectileTimer->StopTimer();
	_waitingTimer->StopTimer();
	_target = nullptr;
	_actionBlock = nullptr;
	_visualSlider = nullptr;
	this->text = new Text();
	UpdateHealthText();
	this->text->color = WHITE;
}

Player::~Player()
{

}

void Player::Update(float deltaTime) 
{
	Move(deltaTime);
	switch (gameEntityState)
	{
		case idle:
			// EnableJump(deltaTime);
			return;
		case attacking:
			//std::cout << _attackingTimer->GetSeconds() << std::endl;
			SwitchAttackType(deltaTime);
			return;
		case choosing:
			EnableJump(deltaTime);
			HandleChoosing();
			return;
		case defending:
			EnableJump(deltaTime);
			return;
	}
	
}

void Player::HandleChoosing() 
{
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
			TeleportToPosition(_startPos);
			// Select Action
			_actionBlock->OpenCommandWindow();
			choosingCommand = true;
			choosingAction = false;
		}
		return;
	}
	if (choosingCommand)
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
}

void Player::SwitchAttackType(float deltaTime)
{
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
			EnableJump(deltaTime);
			HandleJumpAttack(deltaTime);
			return;
		case 3:
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
			TeleportToPosition(_startPos);
			MashProjectileAttack(_target);
			return;
		case 2:
			TeleportToPosition(_startPos);
			JumpAttack(_target);
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
	if (GetInput()->GetKeyDown(_actionKey) && _isgrounded)
	{
		_velocity = glm::vec3(0, 0, 0);
		Jump(800.0f, 1);
	}
}

void Player::GroundCheck()
{
	if (this->gameEntityState == attacking)
	{
		if (this->position.y > _target->position.y && !_isgrounded)
		{
			_isgrounded = true;
			_shouldFall = false;
			_velocity = glm::vec3(0, 0, 0);
		}
		return;
	}
	if (this->position.y < 0 && !_isgrounded || this->position.y > _startPos.y && !_isgrounded)
	{
		_isgrounded = true;
		_shouldFall = false;
		_velocity = glm::vec3(0, 0, 0);
		TeleportToPosition(_startPos);
	}
}

void Player::HandlePunch() 
{
	if (GetInput()->GetKeyDown(_actionKey))
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
	if (GetInput()->GetKeyDown(_actionKey))
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

void Player::HandleJumpAttack(float deltaTime)
{
	//first move towards the enemy
	if (this->position.x >= _target->position.x - 256.0f && _isgrounded)
	{
		//second, simulate a jump.
		Jump(750.0f, 1.5f);
		return;
	}
	//third, handle the attack. It jumps on the enemy three times if the button presses are timed correctly
	if (_jumpAttacksHit <= 3 && this->position.x >= _target->position.x) //if the enemy has not been juped on 3 times yet
	{
		if (this->position.y + this->sprite->GetHeight() / 2.0f >= _target->position.y - _target->sprite->GetHeight() / 1.5f)//calculate if the player collides with the enemy
		{
			if (GetInput()->GetKeyDown(_actionKey))
			{
				this->position.y = _target->position.y - _target->sprite->GetHeight() / 2.0f;
				_velocity = glm::vec3(0, 0, 0);
				Jump(700.0f, 1);
				DealDamage(_target, 1 * (_jumpAttacksHit * 0.5f));
				_jumpAttacksHit++;
				return;
			}
		}
		if (this->position.y >= _target->position.y - _target->sprite->GetHeight() / 4.0f)//calculate if the player collides with the enemy
		{
			ResetToIdle();
		}
	}
	if (_jumpAttacksHit == 3)
	{
		//fourth, when failing or succeeding, when done with the attack go back to the starting position
		_isgrounded = true;
		_shouldFall = false;
		_velocity = glm::vec3(0, 0, 0);
		_jumpAttacksHit = 0;
		ResetToIdle();
	}
	if (this->position.x >= _target->position.x) return; //when you are above the enemy

	MoveTowardsPosition(_initialTargetVector, deltaTime); //moving towards the enemy position
}


void Player::SelectBlock()
{
	_actionBlock->OpenCommandWindow();
}

void Player::BasicAttack(int damage, GameEntity* target)
{
	target->health -= damage; //basic
}

void Player::UpdateHealthText()
{
	this->text->text = "Player: " + std::to_string(this->GetID()) + " HP: " + std::to_string(health) + " / " + std::to_string(_maxHealth);
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
	_turnManager->battleText->text = "Press the [Action Key] when the bar above your head is full!";
}


void Player::MashProjectileAttack(GameEntity* target)
{
	ClearHUD();
	this->_velocity = glm::vec3(0, 0, 0);
	gameEntityState = attacking;
	InitiateVisualSlider();
	_attackingTimer->StartOverTimer();
	_target = target;
	_turnManager->battleText->text = "MASH THE [Action Key] TO FIRE A STRONG PROJECTILE!";
}

void Player::JumpAttack(GameEntity* target)
{
	ClearHUD();
	_target = target;
	_initialTargetVector = ObtainNormalizedVector(_target->position);
	gameEntityState = attacking;
	_attackingTimer->StartOverTimer();
	_turnManager->battleText->text = "Press the [Action Key] when landing on its head!";
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


void Player::AssignActionKey(int jumpKey)
{
	this->_actionKey = jumpKey;
}

void Player::ResetToIdle()
{
	_velocity = glm::vec3(0, 0, 0);
	TeleportToPosition(_startPos);
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
	if (!_isgrounded && _shouldFall)
	{
		_velocity.y += _gravity * deltaTime;
	}
	GroundCheck();
}

void Player::Jump(float jumpForce, float jumpforceMultiplier)
{
	_velocity -= glm::vec3(0.0f, jumpForce * jumpforceMultiplier, 0.0f); //substract from velocity because 0,0 is in the top left
	_ascendTimer->StartOverTimer();
	_isgrounded = false;
	_shouldFall = false;
}
