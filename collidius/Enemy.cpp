#include "Enemy.h" 
#include "TurnManager.h" 

Enemy::Enemy(unsigned char id, short health, unsigned char power, unsigned char defense, unsigned char speed, float damageReduction, unsigned char criticalChance)
	: GameEntity(id,health,power,defense,speed,damageReduction,criticalChance) //Call the GameEntity constructor
{
	//Scale the hitbox up because the enemy is bigger
	hitBox->scale = glm::vec3(2,2,0);
}

Enemy::~Enemy()
{

}

void Enemy::Update(float deltaTime)
{
	GameEntity::Update(deltaTime);
	hitBox->position = this->position;
	switch (gameEntityState)
	{
		case attacking:
			HandleAttacking(deltaTime);
			return;
		case choosing:
			HandleChoosing(deltaTime);
			return;
	}
}

void Enemy::HandleAttacking(float deltaTime)
{
	//Rotate the star indicator
	_starIndicator->rotation += 5 * deltaTime;

	//If the star indicator scale is bigger than or equal to 0
	if (_starIndicator->scale.x >= 0)
	{
		_starIndicator->scale -= 1.5f * deltaTime; //Scale it down
	}
	if (_target->GetID() == 1) //Simulate the star indicator going up when this entity is targetting player 1
	{
		_starIndicator->position -= glm::vec3(0, 128, 0) * deltaTime;
	}
	if (_waitingTimer->GetSeconds() <= 1 && _waitingTimer->isPlaying)return; //Wait for the timer to reach one second to proceed

	//If the attack has not been performed yet, fire a projectile attacks
	if (!_attackPerformed)
	{
		_waitingTimer->StopTimer();
		ProjectileAttack(_target);
		_attackPerformed = true;
		return;
	}

	//Check if the projectile(s) collide if the list is not empty
	if (_projectiles.size() > 0)
	{
		for (size_t i = 0; i < _projectiles.size(); i++)
		{
			HandleProjectileCollision(_projectiles[i], _target);
		}
		return;
	}

	//Once the list is empty and the attack has been performed
	_waitingTimer->StopTimer(); //Stop the timer
	_target->gameEntityState = _target->idle; //Set the game entity state to idle
	completedTurn = true; //Complete the turn
	gameEntityState = idle; //Set the game entity state to idle

	//Destroy the star indicator
	this->RemoveChild(_starIndicator);
	delete _starIndicator;
	_starIndicator = nullptr;
}

void Enemy::HandleChoosing(float deltaTime)
{
	//for now default to attacking, healing might come into play later
	_waitingTimer->StartOverTimer();
	gameEntityState = attacking; //Automatically set the state to attacking
	_target = TurnManager::Instance()->GetRandomPlayer(); //Get random player to set as target
	_attackPerformed = false;

	//Create the star indicator
	_starIndicator = new Entity();
	_starIndicator->SetSprite("assets/sprites/starindicator.tga");
	this->AddChild(_starIndicator);
	_starIndicator->scale = glm::vec3(1.5f, 1.5f, 0);

	if (_target->GetID() == 1)
	{
		//If the player targeted is player 1, set star indicator sprite to the right eye from gorilla perspective
		_starIndicator->position = this->position - glm::vec3(32, 32, 0);
		return;
	}
	//If the player targeted is player 2, set star indicator sprite to the left eye from gorilla perspective
	_starIndicator->position = this->position - glm::vec3(-4, 32, 0);
}

void Enemy::UpdateHealthText()
{
	this->text->text = "Enemy: " + std::to_string(this->GetID()) + " HP: " + std::to_string(health) + " / " + std::to_string(GetMaxHealth());
}

void Enemy::ProjectileAttack(GameEntity* target)
{
	//Create a projectile
	FireProjectile(target);
	_target->gameEntityState = _target->defending; //Make sure the right player can defend
	TurnManager::Instance()->battleText->text = "WATCH OUT!";
}

void Enemy::FireProjectile(GameEntity* target)
{
	//Make eye shine corresponding to the player position
	_target = target;

	//Create random bool for later
	bool randomBool = rand() % 2;

	//Create projectile
	Projectile* projectile = new Projectile(this->position, 700, _shouldDisplayHitboxes);
	projectile->scale = glm::vec3(2, 2, 0);
	this->AddChild(projectile);
	_projectiles.push_back(projectile);
	projectile->SetSprite("assets/sprites/bananarang.tga");

	//If the random bool is false, target the player normally
	if (randomBool == 0)
	{
		projectile->SetTarget(_target->GetStartPos());
		return;
	}

	//If the random bool is true, target above the player
	if (_target->GetID() == 1)
	{
		//Target higher than player 2 because of positioning
		projectile->SetTarget(_target->GetStartPos() - glm::vec3(0, 172, 0)); 
		return;
	}
	//Target above player 2
	projectile->SetTarget(_target->GetStartPos() - glm::vec3(0, 140, 0));
}

void Enemy::HandleProjectileAction()
{
	//On collision death, deal damage
	DealDamage(_target, 1);
}
