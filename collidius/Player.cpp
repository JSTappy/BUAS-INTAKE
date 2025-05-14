#include "Player.h" 
#include "TurnManager.h" 

Player::Player(unsigned char id, short health, unsigned char power, unsigned char defense, unsigned char speed, float damageReduction, unsigned char criticalChance)
	: GameEntity(id, health, power, defense, speed, damageReduction, criticalChance) //Call the GameEntity constructor
{
	_actionKey = 0; //Set this later via public method

	//Set grounded
	_isGrounded = true;
	_shouldFall = false;

	_ascendTimer = new Timer();
	_attackingTimer = new Timer();

	//Add the timers so they are able to update
	this->AddChild(_ascendTimer);
	this->AddChild(_attackingTimer);

	//Stop the timers
	_ascendTimer->StopTimer();
	_attackingTimer->StopTimer();

	//These get assigned later
	_target = nullptr;
	_actionBlock = nullptr;
	_visualSlider = nullptr;
}

Player::~Player()
{
	this->RemoveChild(_waitingTimer);
	this->RemoveChild(_attackingTimer);
	this->RemoveChild(_ascendTimer);
}

void Player::Update(float deltaTime) 
{
	GameEntity::Update(deltaTime);
	hitBox->position = this->position; //Make sure the hitbox follows the player
	if (_visualSlider) //Make sure the visual slider follows the player
	{
		_visualSlider->position.x = this->position.x;
		_visualSlider->position.y = this->position.y - 100;
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
			if (_turnsToSkip > 0) //Skip a turn if the player has to
			{
				_turnsToSkip--;
				ResetToIdle();
				return;
			}
			EnableJump(deltaTime);
			HandleChoosing();
			return;
		case defending: //Defending state, make sure the player can jump to avoid enemy attacks
			EnableJump(deltaTime);
			return;
	}
}

void Player::AssignActionKey(unsigned char jumpKey)
{
	this->_actionKey = jumpKey;
}

void Player::UpdateHealthText()
{
	//Set the health text to the player health
	this->text->text = "Player: " + std::to_string(this->GetID()) + " HP: " + std::to_string(health) + " / " + std::to_string(GetMaxHealth());
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
	float randomPitch = 0.75f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
	_jumpSound->setPitch(randomPitch);
	_jumpSound->replay();
	_velocity -= glm::vec3(0, jumpForce * jumpforceMultiplier, 0); //Substract from velocity because 0,0 is in the top left
	_ascendTimer->StartOverTimer();
	_isGrounded = false;
	_shouldFall = false;
}

void Player::EnableJump(float deltaTime) 
{
	if (GetInput()->GetKeyDown(_actionKey) && _isGrounded) //If the action key is pressed and you are on the ground
	{
		_velocity = glm::vec3(0, 0, 0); //Reset velocity
		Jump(800, 1);
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
	unsigned char actionType = _actionBlock->ConfirmAction().x; //The type of action chosen, Attack (0), Defense (1), Item (2), Special (3)
	unsigned char actionIndex = _actionBlock->ConfirmAction().y; //The index of the chosen action type, 0, 1, 2, 3

	switch (actionType)
	{
		case 0: //Attacking action
			PerformAttack(actionIndex);
			break;
		case 1: //Defending action
			PerformDefenseAction(actionIndex);
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

void Player::PerformAttack(unsigned char attackLevel)
{
	_attackType = attackLevel;
	_target = TurnManager::Instance()->GetEnemy();
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

void Player::PerformDefenseAction(unsigned char defenseLevel)
{
	switch (defenseLevel) //Check to see which defense action should be performed
	{
		case 0: //Increase defense by 1
			this->defense += 1;
			ResetToIdle();
			return;
		case 1: //15% damage reduction, but you skip a turn
			this->_damageReduction = 0.15f;
			this->_turnsToSkip = 1;
			ResetToIdle();
			return;
		case 2: //Defense for power trade-off
			this->defense += 4;
			this->power -= 4;
			ResetToIdle();
			return;
		case 3: //50% damage reduction, but you skip 3 turns
			this->_damageReduction = 0.5f;
			this->_turnsToSkip = 3;
			ResetToIdle();
			return;
	}
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
		if (_attackingTimer->GetSeconds() < 1) //If the timing was too early
		{
			TurnManager::Instance()->battleText->text = "A little too early...";
			DealDamage(_target, 0.3f);
			ResetToIdle();
			return;
		}
		if (_attackingTimer->GetSeconds() > 1 && _attackingTimer->GetSeconds() < 1.5f) //If the timing was spot on
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
	if (_attackingTimer->GetSeconds() >= 2) //If you did not press the button in time
	{
		TurnManager::Instance()->battleText->text = "You Failed to attack...";
		ResetToIdle();
		return;
	}

	if (this->position.x >= _target->position.x - 256) return; //When you are close to the enemy, don't move towards it

	MoveTowardsPosition(_target->position - glm::vec3(_initialTargetVector.x + 64, _target->position.y, 0), 800, deltaTime); //Move towards the enemy
}

void Player::HandleJumpAttack(float deltaTime)
{
	if (this->position.x >= _target->position.x - 256 && _isGrounded) //check if the player should jump towards the enemy already
	{
		Jump(750, 1.25f);
		return;
	}
	//third, handle the attack. It jumps on the enemy three times if the button presses are timed correctly
	if (_jumpAttacksHit <= 3 && this->position.x >= _target->position.x) //If the enemy has not been juped on 3 times yet
	{
		if (this->position.y + this->sprite->GetHeight() / 2 >= _target->position.y - _target->sprite->GetHeight() / 2)//calculate if the player collides with the enemy
		{
			if (GetInput()->GetKeyDown(_actionKey)) //When getting the action key while being in range to jump on the enemy
			{
				//Count a jump hit and 
				_velocity = glm::vec3(0, 0, 0);
				Jump(600, 1);
				DealDamage(_target, 0.65f);
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

	MoveTowardsPosition(_initialTargetVector, 600, deltaTime); //Moving towards the enemy position
}

void Player::HandleProjectileMash()
{
	if (_attackingTimer->GetSeconds() >= 1.5f) //after 1.5 seconds, do the calculations
	{
		//Check how much charge is stored
		if (_energyStored <= 1) { _energyStored = 0; } //Lower than or 1
		if (_energyStored >= 1 && _energyStored < 2) { _energyStored = 1; } //Higher than 1, lower than 2
		if (_energyStored >= 2 && _energyStored < 3) { _energyStored = 2; } //Higher than 2, lower than 3
		if (_energyStored >= 3) { _energyStored = 3; } //Higher than or 3

		//spawn projectile you fire at the opponent
		this->FireProjectile(_target);
		_attackPerformed = true;
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
		_energyStored += 0.25f;
		_visualSlider->UpdateSliderSpriteOnKeys(0.25f, 3);
	}
}

void Player::HandleDashAttack(float deltaTime)
{
	if (_attackingTimer->GetSeconds() > 2)  //If you did nothing within 2 seconds
	{
		if (TurnManager::Instance()->battleText->text != "You Charged for too long...") { TurnManager::Instance()->battleText->text = "You Charged for too long..."; }
		MoveTowardsPosition(_initialTargetVector, 600, deltaTime); //Moving towards the enemy position
		if (this->position.x + this->sprite->GetWidth() / 2 >= _target->position.x - _target->sprite->GetWidth() / 2) //if the player hits the enemy
		{
			this->defense -= this->defense / 10; //only substract defense from the player
			TurnManager::Instance()->battleText->text = "Player defense - 10%";
			DealDamage(_target, 0.01f); //deal little damage
			ResetToIdle();
		}
		return;
	}
	if (_attackPerformed) //if the attack has been performed
	{
		if (_energyStored > 1) //Move faster towards the enemy with more charge
		{
			MoveTowardsPosition(_initialTargetVector, 400 * _energyStored, deltaTime); //Moving towards the enemy position
		}
		else
		{
			MoveTowardsPosition(_initialTargetVector, 400, deltaTime); //Moving towards the enemy position
		}
		if (this->position.x + this->sprite->GetWidth() / 2 >= _target->position.x - _target->sprite->GetWidth() / 2) //If the player hits the enemy
		{
			//Substract both player and Enemy defenses
			this->defense -= this->defense / 10;
			_target->defense -= _target->defense / 10;
			TurnManager::Instance()->battleText->text = "Player & enemy defense - 10%";
			if (_energyStored > 2.25f) //Cap out damage at 2.25 for balancing
			{
				DealDamage(_target, 2.25f);
				ResetToIdle();
				return;
			}
			DealDamage(_target, _energyStored);
			ResetToIdle();
		}
		return;
	}
	if (GetInput()->GetKeyUp(_actionKey)) //When the key is released after its been pressed / held down
	{
		_attackPerformed = true;
		_attackingTimer->StopTimer();
	}
	if (GetInput()->GetKey(_actionKey)) //When holding the Key
	{
		_energyStored += 1.75f * deltaTime;
		_visualSlider->UpdateSliderSpriteOnKeys(1.75f * deltaTime, 2.5f);
	}
}

void Player::UseItem(unsigned char index)
{
	//Check if the indexed item can still be used
	if (_items[index]->GetUses() == 0)
	{
		TeleportToPosition(_startPos);
		TurnManager::Instance()->battleText->text = "You do not have this item anymore! Pick something else";
		return;
	}
	_items[index]->ApplyItem(this); //Use the item
	TurnManager::Instance()->battleText->text = "You can use this item " + std::to_string(_items[index]->GetUses()) + " more time(s)";
	ResetToIdle();
}


void Player::SwitchAttackType(float deltaTime)
{
	if (_waitingTimer->GetSeconds() <= 1)return; //Wait for a second
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
			if (!_attackPerformed) //If the player has not shot his shot yet
			{
				HandleProjectileMash(); //Handle the projectile attack set up
				return;
			}
			if (_projectiles.size() > 0) //If there are projectiles
			{
				for (size_t i = 0; i < _projectiles.size(); i++)
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
			_actionBlock->position = this->_startPos - glm::vec3(0, 200, 0);
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
		if (GetInput()->GetKeyDown(TAB)) //Go back to the action block choosing
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
	_visualSlider->position = this->position - glm::vec3(0, 128, 0);
	_visualSlider->scale = glm::vec3(1, 0.5f, 1);
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
	_attackPerformed = false;
	_energyStored = 0;
	_jumpAttacksHit = 0;

	//Complete the turn
	completedTurn = true;
	gameEntityState = idle;

	//Clear the screen
	ClearHUD();
}
