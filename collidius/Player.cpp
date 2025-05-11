#include "Player.h" 
#include "TurnManager.h" 

Player::Player(int id, int level, float health, int power, int defense, int speed, float damageReduction, int criticalChance, int money, int experiencePoints)
	: GameEntity(id, level, health, power, defense, speed, damageReduction, criticalChance, money, experiencePoints) //Call the GameEntity constructor
{
	_actionKey = 0; //Set this later via public method

	//Set grounded
	_isGrounded = true;
	_shouldFall = false;

	_ascendTimer = new Timer();
	_attackingTimer = new Timer();
	_waitingTimer = new Timer();

	//Add the timers so they are able to update
	this->AddChild(_waitingTimer);
	this->AddChild(_attackingTimer);
	this->AddChild(_ascendTimer);

	//Stop the timers
	_ascendTimer->StopTimer();
	_attackingTimer->StopTimer();
	_waitingTimer->StopTimer();

	//These get assigned later
	_target = nullptr;
	_actionBlock = nullptr;
	_visualSlider = nullptr;
}

Player::~Player()
{

}

void Player::Update(float deltaTime) 
{
	hitBox->position = this->position; //Make sure the hitbox follows the player
	if (_visualSlider) //Make sure the visual slider follows the player
	{
		_visualSlider->position.x = this->position.x;
		_visualSlider->position.y = this->position.y - 100.0f;
	}
	hitBox->scale = this->scale;
	Move(deltaTime); //Apply velocity and gravity
	switch (gameEntityState)
	{
		case idle: //Idle state, do nothing
			return;
		case attacking: //Attacking state, handle the attack through the SwitchAttackType function
			SwitchAttackType(deltaTime);
			return;
		case choosing: //Choosing state, make sure the player can jump and choose an action to perform
			EnableJump(deltaTime);
			HandleChoosing();
			return;
		case defending: //Defending state, make sure the player can jump to avoid enemy attacks
			EnableJump(deltaTime);
			return;
	}
	
}

void Player::AssignActionKey(int jumpKey)
{
	this->_actionKey = jumpKey;
}

void Player::UpdateHealthText()
{
	//Set the health text to the player health
	this->text->text = "Player: " + std::to_string(this->GetID()) + " HP: " + std::to_string(health) + " / " + std::to_string(_maxHealth);
}

void Player::Move(float deltaTime)
{
	this->position += _velocity * deltaTime; //Apply velocity to the position
	if (_ascendTimer->GetSeconds() > 0.20f) //When being aerial for longer than 1/5th of a second, stop ascending
	{
		_ascendTimer->StopTimer();
		_shouldFall = true;
	}
	if (!_isGrounded && _shouldFall && gameEntityState == attacking) //Jump Attack exclusive gravity logic
	{
		_velocity.y += (_gravity * 0.8f) * deltaTime;
		return;
	}
	if (!_isGrounded && _shouldFall) //When aerial and when you should be falling
	{
		_velocity.y += _gravity * deltaTime;
	}
	GroundCheck();
}

void Player::Jump(float jumpForce, float jumpforceMultiplier)
{
	_velocity -= glm::vec3(0.0f, jumpForce * jumpforceMultiplier, 0.0f); //Substract from velocity because 0,0 is in the top left
	_ascendTimer->StartOverTimer();
	_isGrounded = false;
	_shouldFall = false;
}

void Player::EnableJump(float deltaTime) 
{
	if (GetInput()->GetKeyDown(_actionKey) && _isGrounded) //If the action key is pressed and you are on the ground
	{
		_velocity = glm::vec3(0, 0, 0); //Reset velocity
		Jump(800.0f, 1);
	}
}

void Player::GroundCheck()
{
	if (this->gameEntityState == attacking) //Special for moving attacks such as punching and jumping
	{
		if (_waitingTimer->GetSeconds() < 1.25f && _waitingTimer->isPlaying) { _isGrounded = true; return; } //If the waiting timer is below 1.25 seconds, you are still 'grounded'
		if (this->position.y > _target->position.y && !_isGrounded) //if the Y position of the player is bigger (lower) than the enemy
		{
			_isGrounded = true;
			_shouldFall = false;
			_velocity = glm::vec3(0, 0, 0);
		}
		return; //No need to check the next if statement if the above is true
	}
	if (this->position.y > _startPos.y && !_isGrounded) //if the players position is bigger (lower) than the starting position and you are not grounded
	{
		//Make sure no forces are being applied to the player and immedeatly set the players position back to the starting position
		_isGrounded = true;
		_shouldFall = false;
		_velocity = glm::vec3(0, 0, 0);
		TeleportToPosition(_startPos); 
	}
}

void Player::SelectBlock()
{
	_actionBlock->OpenCommandWindow(); //Open the command window to select a specific action of an action type
}

void Player::HandleAction()
{
	int actionType = _actionBlock->ConfirmAction().x; //The type of action chosen, Attack (0), Defense (1), Item (2), Special (3)
	int actionIndex = _actionBlock->ConfirmAction().y; //The index of the chosen action type, 0, 1, 2, 3

	switch (actionType)
	{
		case 0: //Attacking action
			PerformAttack(actionIndex);
			break;
		case 1: //Defending action
			//TODO
			this->_damageReduction = 0.3f;
			ResetToIdle();
			return;
		case 2: //Item action
			UseItem(actionIndex);
			break;
		case 3:	//Special action
			//TODO
			break;
	}
}

void Player::HandleProjectileAction()
{
	DealDamage(_target, _energyStored / 2);
}


void Player::PerformAttack(int attackLevel)
{
	_attackType = attackLevel;
	_target = TurnManager::Instance()->GetRandomEnemy();
	_waitingTimer->StartOverTimer();
	switch (_attackType) //Check to see which attack should be performed
	{
		case 0: //Punch Attack
			PunchAttack(_target);
			return;
		case 1: //Jump Attack
			TeleportToPosition(_startPos);
			JumpAttack(_target);
			return;
		case 2: //Projectile Attack
			TeleportToPosition(_startPos);
			MashProjectileAttack(_target);
			return;
		case 3: //Dash Attack
			DashAttack(_target);
			return;
	}
}

void Player::BasicAttack(int damage, GameEntity* target)
{
	target->health -= damage; //basic
}

void Player::PunchAttack(GameEntity* target)
{
	//Set up the player for the attack
	ClearHUD();
	this->_velocity = glm::vec3(0, 0, 0);
	this->TeleportToPosition(_startPos);
	InitiateVisualSlider();
	gameEntityState = attacking;
	_target = target;

	//Get the normalized vector between the player and the target so you can move towards it automatically
	_initialTargetVector = ObtainNormalizedVector(_target->position);
	TurnManager::Instance()->battleText->text = "Press the [Action Key] when the bar above your head is full!";
}

void Player::JumpAttack(GameEntity* target)
{
	//Set up the player for the attack
	ClearHUD();
	_target = target;
	_initialTargetVector = ObtainNormalizedVector(_target->position);
	gameEntityState = attacking;
	TurnManager::Instance()->battleText->text = "Press the [Action Key] when landing on its head!";
}

void Player::MashProjectileAttack(GameEntity* target)
{
	//Set up the player for the attack
	ClearHUD();
	this->_velocity = glm::vec3(0, 0, 0);
	gameEntityState = attacking;
	InitiateVisualSlider();
	_target = target;
	TurnManager::Instance()->battleText->text = "MASH THE [Action Key] TO FIRE A STRONG PROJECTILE!";
}

void Player::DashAttack(GameEntity* target)
{
	//Set up the player for the attack
	ClearHUD();
	_target = target;
	this->_velocity = glm::vec3(0, 0, 0);
	this->TeleportToPosition(_startPos);

	_initialTargetVector = ObtainNormalizedVector(_target->position);
	gameEntityState = attacking;
	InitiateVisualSlider();
	TurnManager::Instance()->battleText->text = "Hold the [Action Key] and Release it when the timing is right!";
}

void Player::HandlePunch(float deltaTime)
{
	if (GetInput()->GetKeyDown(_actionKey)) //Check if the action key has been pressed
	{
		if (_attackingTimer->GetSeconds() < 1.0f) //If the timing was too early
		{
			TurnManager::Instance()->battleText->text = "A little too early...";
			DealDamage(_target, 0.3);
			ResetToIdle();
			return;
		}
		if (_attackingTimer->GetSeconds() > 1.0f && _attackingTimer->GetSeconds() < 1.5f) //If the timing was spot on
		{
			TurnManager::Instance()->battleText->text = "PERFECT!!";
			DealDamage(_target, 1);
			ResetToIdle();
			return;
		}
		if (_attackingTimer->GetSeconds() > 1.5f) //If the timing was too late
		{
			TurnManager::Instance()->battleText->text = "You were too late...";
			ResetToIdle();
			return;
		}
	}
	if (_attackingTimer->GetSeconds() >= 2.0f) //If you did not press the button in time
	{
		TurnManager::Instance()->battleText->text = "You Failed to attack...";
		ResetToIdle();
		return;
	}

	if (this->position.x >= _target->position.x - 256.0f) return; //When you are close to the enemy, don't move towards it

	MoveTowardsPosition(_target->position - glm::vec3(_initialTargetVector.x + 64.0f, _target->position.y, 0), 800.0f, deltaTime); //Move towards the enemy
}

void Player::HandleJumpAttack(float deltaTime)
{
	if (this->position.x >= _target->position.x - 256.0f && _isGrounded) //check if the player should jump towards the enemy already
	{
		Jump(750.0f, 1.25f);
		return;
	}
	//third, handle the attack. It jumps on the enemy three times if the button presses are timed correctly
	if (_jumpAttacksHit <= 3 && this->position.x >= _target->position.x) //If the enemy has not been juped on 3 times yet
	{
		if (this->position.y + this->sprite->GetHeight() / 2.0f >= _target->position.y - _target->sprite->GetHeight() / 2.0f)//calculate if the player collides with the enemy
		{
			if (GetInput()->GetKeyDown(_actionKey)) //When getting the action key while being in range to jump on the enemy
			{
				//Count a jump hit and 
				_velocity = glm::vec3(0, 0, 0);
				Jump(600.0f, 1);
				DealDamage(_target, 1 * _jumpAttacksHit);
				_jumpAttacksHit++;
				return;
			}
		}
		if (this->position.y >= _target->position.y - _target->sprite->GetHeight() / 2.25f)//calculate if the player collides with the enemy
		{
			ResetToIdle();
		}
	}
	if (_jumpAttacksHit == 3) { ResetToIdle(); } //When hitting 3 jump attacks, go back to Idle state
	if (this->position.x >= _target->position.x) return; //When you are above the enemy

	MoveTowardsPosition(_initialTargetVector, 600.0f, deltaTime); //Moving towards the enemy position
}

void Player::HandleProjectileMash()
{
	if (_attackingTimer->GetSeconds() >= 1.5f) //after 1.5 seconds, do the calculations
	{
		//Check how much charge is stored
		if (_energyStored <= 1.0f) { _energyStored = 0.0f; } //Lower than or 1
		if (_energyStored >= 1.0f && _energyStored < 2.0f) { _energyStored = 1.0f; } //Higher than 1, lower than 2
		if (_energyStored >= 2.0f && _energyStored < 3.0f) { _energyStored = 2.0f; } //Higher than 2, lower than 3
		if (_energyStored >= 3.0f) { _energyStored = 3.0f; } //Higher than or 3

		//spawn projectile you fire at the opponent
		this->FireProjectile(_target, 1, 1);
		_shotsFired = true;
		if (this->health > this->GetMaxHealth() / 10) //take 10% damage standard
		{
			this->health -= this->GetMaxHealth() / 10;
			this->UpdateHealthText();
			return;
		}
		this->health = 1;
		this->UpdateHealthText();
		return;
	}
	if (GetInput()->GetKeyDown(_actionKey)) //mash the action key to gain charge
	{
		_energyStored += 0.25;
		_visualSlider->UpdateSliderSpriteOnClicks(0.25, _energyMax);
	}
}

void Player::HandleDashAttack(float deltaTime)
{
	if (_attackingTimer->GetSeconds() > 2.0f) 
	{
		if (TurnManager::Instance()->battleText->text != "You Charged for too long...") { TurnManager::Instance()->battleText->text = "You Charged for too long..."; }
		MoveTowardsPosition(_initialTargetVector, 600.0f, deltaTime); //Moving towards the enemy position
		if (this->position.x + this->sprite->GetWidth() / 2.0f >= _target->position.x - _target->sprite->GetWidth() / 2.0f)
		{
			this->defense -= this->defense / 10;
			TurnManager::Instance()->battleText->text = "Player defense - 10%";
			_target->health -= 1;
			_target->UpdateHealthText();
			ResetToIdle();
		}
		return;
	}
	if (_shotsFired)
	{
		if (_energyStored > 1)
		{
			MoveTowardsPosition(_initialTargetVector, 400.0f * _energyStored, deltaTime); //Moving towards the enemy position
		}
		else
		{
			MoveTowardsPosition(_initialTargetVector, 400.0f, deltaTime); //Moving towards the enemy position
		}
		if (this->position.x + this->sprite->GetWidth() / 2.0f >= _target->position.x - _target->sprite->GetWidth() / 2.0f)
		{
			this->defense -= this->defense / 10;
			_target->defense -= _target->defense / 10;
			TurnManager::Instance()->battleText->text = "Player & enemy defense - 10%";
			if (_energyStored > 2.25)
			{
				DealDamage(_target, 2.25);
				ResetToIdle();
				return;
			}
			DealDamage(_target, _energyStored);
			ResetToIdle();
		}
		return;
	}
	if (GetInput()->GetKeyUp(_actionKey))
	{
		_shotsFired = true;
	}

	if (GetInput()->GetKey(_actionKey))
	{
		_energyStored += 0.0025;
		_visualSlider->UpdateSliderSpriteOnClicks(0.0025, _energyMax);
	}
}

void Player::UseItem(int index)
{
	//Check if the indexed item can still be used
	if (_items[index]->GetUses() == 0)
	{
		TeleportToPosition(_startPos);
		TurnManager::Instance()->battleText->text = "You do not have this item anymore! Pick something else";
		return;
	}
	_items[index]->ApplyItem(this);
	TurnManager::Instance()->battleText->text = "You can use this item " + std::to_string(_items[index]->GetUses()) + " more time(s)";
	ResetToIdle();
}


void Player::SwitchAttackType(float deltaTime)
{
	if (_waitingTimer->GetSeconds() <= 1.0f)return; //Wait for a second
	if (!_attackingTimer->isPlaying)_attackingTimer->StartOverTimer(); //Make sure the attack timer is playing
	switch (_attackType)
	{
		case 0:
			_visualSlider->UpdateSliderSpriteOnTime(_attackingTimer->GetSeconds()); //Fill the bar
			HandlePunch(deltaTime); //Handle the punch attack
			return;
		case 1:
			HandleJumpAttack(deltaTime); //Handle the jump attack
			return;
		case 2:
			if (!_shotsFired) //If the player has not shot his shot yet
			{
				HandleProjectileMash(); //Handle the projectile attack set up
				return;
			}
			if (_projectiles.size() > 0) //If there are projectiles
			{
				for (int i = 0; i < _projectiles.size(); i++)
				{
					HandleProjectileCollision(_projectiles[i], _target); //Handle the AABB collision between the target and the player
				}
				return;
			}
			ResetToIdle();
			return;
		case 3:
			HandleDashAttack(deltaTime);
			return;
	}
}

void Player::HandleChoosing() 
{
	if (choosingAction) //When choosing an Action
	{
		if (_actionBlock == nullptr) //Create an actionblock if it does not exist yet
		{
			_actionBlock = new ActionBlock();
			_actionBlock->position = this->_startPos + glm::vec3(0, -200, 0);
			this->AddChild(_actionBlock);
		}
		if (GetInput()->GetKeyDown(KEY_A) && _isGrounded) //Switch between actionblocks
		{
			_actionBlock->PreviousBlock();
		}
		if (GetInput()->GetKeyDown(KEY_D) && _isGrounded) //Switch between actionblocks
		{
			_actionBlock->NextBlock();
		}
		if (this->position.y < _actionBlock->position.y + _actionBlock->sprite->GetHeight()) //If you hit the block, open the command window
		{
			this->_velocity = glm::vec3(0, 0, 0);
			TeleportToPosition(_startPos);

			// Select Action
			_actionBlock->OpenCommandWindow();
			choosingCommand = true;
			choosingAction = false;
		}
		return; //No need to check whats after this if the choosing action condition is true
	}
	if (choosingCommand) //When choosing a command option in the command window
	{
		if (GetInput()->GetKeyDown(Tab)) //Go back to the action block choosing
		{
			ClearHUD();
			choosingAction = true;
			choosingCommand = false;
			return;
		}
		if (this->position.y < _actionBlock->position.y + _actionBlock->sprite->GetHeight()) //When hitting the block, perform the action
		{
			this->_velocity = glm::vec3(0, 0, 0);
			HandleAction();
		}
	}
}

void Player::InitiateVisualSlider()
{
	//create the visual slider
	_visualSlider = new VisualSlider(this->GetID());
	_visualSlider->position = this->position - glm::vec3(0.0f, 128.0f, 0.0f);
	_visualSlider->scale = glm::vec3(1.0f, 0.5f, 1.0f);
	this->AddChild(_visualSlider);
	_visualSlider->InitVisuals();
	_visualSlider->SetSprite("assets/sprites/slider.tga");
}

void Player::ClearHUD() 
{
	if (_actionBlock != nullptr) //Delete the action block if it exists
	{
		this->RemoveChild(_actionBlock);
		_actionBlock->CleanUp();
		delete _actionBlock;
		_actionBlock = nullptr;
	}
	if (_visualSlider != nullptr) //Delete the visual slider if it exists
	{
		this->RemoveChild(_visualSlider);
		_visualSlider->ClearFill();
		delete _visualSlider;
		_visualSlider = nullptr;
	}
}

void Player::ResetToIdle()
{
	//Stop the timers
	_waitingTimer->StopTimer();
	_attackingTimer->StopTimer();

	//Reset to the starting position
	_velocity = glm::vec3(0, 0, 0);
	_isGrounded = true;
	_shouldFall = false;
	TeleportToPosition(_startPos);

	//Reset the projectile attack properties
	_shotsFired = false;
	_energyStored = 0.0f;
	_jumpAttacksHit = 0;

	//Complete the turn
	completedTurn = true;
	gameEntityState = idle;

	//Clear the screen
	ClearHUD();
}
